/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_others.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:23:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 14:54:36 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cleanup_parent(t_cmd_table *cmd)
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
	if (!cmd->cmd_da || cmd->cmd_da->count == 0)
	{
		exec_cleanup_parent(cmd);
		shell->code = 0;
		return ;
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
		exec_apply_redirs(cmd);
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
		errno_report(cmd);
		child_cleanup_and_exit(shell, cmd, map_exec_errno_to_exit(errno));
	}
	waitpid(child, &status, 0);
	shell_update_code_from_status(shell, status);
	exec_cleanup_parent(cmd);
}

/*looping through the command table and execute each command
 */
void	exec_pipeline(t_shell *shell)
{
	t_cmd_table	*cmd;

	cmd = shell->cmd;
	exec_pipe_entry(shell, cmd);
	if (shell->pipeline && shell->pipeline->tmp_fd != -1)
	{
		close(shell->pipeline->tmp_fd);
		shell->pipeline->tmp_fd = -1;
	}
	while (cmd)
	{
		if (cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		if (cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);
		cmd = cmd->next;
	}
}
