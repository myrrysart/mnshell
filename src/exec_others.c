/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_others.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:23:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/14 17:06:12 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_select(t_shell *shell, t_cmd_table *cmd)
{
	const t_builtin builtin_table[BUILTIN_COUNT] = {
		builtin_cd,
		builtin_echo,
		builtin_exit,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
	};
	builtin_table[cmd->cmd_type](shell, cmd);
}

void	exec_child_no_pipe_prep(t_cmd_table *cmd)
{
	exec_apply_redirs(cmd);
}

void	exec_cleanup_parent(t_cmd_table *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}

void	exec_no_pipe(t_shell *shell)
{
	pid_t		child;
	t_cmd_table	*cmd;
	int			status;
	struct stat	st;

	cmd = shell->cmd;
	if (cmd->cmd_type != EXTERNAL)
		return (exec_builtin_with_redirs(shell, cmd));
	if (!cmd->cmd_da || cmd->cmd_da->count == 0) {
		exec_cleanup_parent(cmd);
		shell->code = 0;
		return;
	}
	child = fork();
	if (child < 0)
	{
		perror("fork");
		shell->code = errno;
		exec_cleanup_parent(cmd);
		return ;
	}
	if (child == 0)
	{
		setup_child_signals();
		exec_child_no_pipe_prep(cmd);
		if (cmd->cmd_da && cmd->cmd_da->count == 1
			&& ft_strncmp(cmd->cmd_da->items[0], ".", 2) == 0)
		{
			ft_putendl_fd(".: filename argument required", 2);
			child_cleanup_and_exit(shell, cmd, EXIT_BUILTIN_MISUSE);
		}
		if (cmd->cmd_da && cmd->cmd_da->items[0])
		{
			if (stat(cmd->cmd_da->items[0], &st) == 0 && S_ISDIR(st.st_mode))
			{
				ft_putstr_fd(cmd->cmd_da->items[0], 2);
				ft_putendl_fd(": Is a directory", 2);
				child_cleanup_and_exit(shell, cmd, EXIT_CMD_NOT_EXECUTABLE);
			}
		}
			if (!cmd->cmd_da || !cmd->cmd_da->items[0])
				child_cleanup_and_exit(shell, cmd, OK);
		execve(cmd->cmd_da->items[0], cmd->cmd_da->items, shell->heap_env);
		if (errno == EACCES)
		{
			ft_putstr_fd(cmd->cmd_da->items[0], 2);
			ft_putendl_fd(": Permission denied", 2);
		}
		else if (errno == ENOENT)
		{
			ft_putstr_fd(cmd->cmd_da->items[0], 2);
			if (ft_strchr(cmd->cmd_da->items[0], '/'))
				ft_putendl_fd(": No such file or directory", 2);
			else
				ft_putendl_fd(": Command not found.", 2);
		}
		else if (errno == ENOTDIR)
		{
			ft_putstr_fd(cmd->cmd_da->items[0], 2);
			ft_putendl_fd(": Not a directory", 2);
		}
		else if (errno == EISDIR)
		{
			ft_putstr_fd(cmd->cmd_da->items[0], 2);
			ft_putendl_fd(": Is a directory", 2);
		}
		else if (errno == ENOEXEC)
		{
			ft_putstr_fd(cmd->cmd_da->items[0], 2);
			ft_putendl_fd(": Exec format error", 2);
		}
		child_cleanup_and_exit(shell, cmd, map_exec_errno_to_exit(errno));
	}
	waitpid(child, &status, 0);
	shell_update_code_from_status(shell, status);
	exec_cleanup_parent(cmd);
}

/*looping through the command table and execute each command
 */
void	exec_pipe(t_shell *shell)
{
	t_cmd_table	*cmd;
	pid_t		child;
	int			status;

	cmd = shell->cmd;
	while (cmd)
	{
		child = exec_pipeline(shell, cmd);
		cmd = cmd->next;
		waitpid(child, &status, 0);
		shell_update_code_from_status(shell, status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			if (shell->pipeline && shell->pipeline->tmp_fd != -1)
			{
				close(shell->pipeline->tmp_fd);
				shell->pipeline->tmp_fd = -1;
			}
			shell_abort_eval(shell, EXIT_SIGINT);
			break ;
		}
		if (!(shell->state & EVALUATING))
		{
			if (shell->pipeline && shell->pipeline->tmp_fd != -1)
			{
				close(shell->pipeline->tmp_fd);
				shell->pipeline->tmp_fd = -1;
			}
			break ;
		}
	}
	if (shell->pipeline && shell->pipeline->tmp_fd != -1)
	{
		close(shell->pipeline->tmp_fd);
		shell->pipeline->tmp_fd = -1;
	}
	cmd = shell->cmd;
	while (cmd)
	{
		if (cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		if (cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);
		cmd = cmd->next;
	}
}
