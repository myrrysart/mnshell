/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_others.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:23:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 16:27:55 by trupham          ###   ########.fr       */
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

void	fork_error(t_shell *shell, t_cmd_table *cmd)
{
	perror("fork");
	shell->code = errno;
	exec_cleanup_parent(cmd);
}

static void	exec_child_prep(t_shell *shell, t_cmd_table *cmd)
{
	struct stat	st;

	sig_mode_set(SIGMODE_CHILD);
	exec_apply_redirs(cmd);
	if (cmd->cmd_da && cmd->cmd_da->count == 1
		&& ft_strncmp(cmd->cmd_da->items[0], ".", 2) == 0)
	{
		ft_putendl_fd(".: filename argument required", 2);
		child_cleanup_and_exit(shell, cmd, EXIT_BUILTIN_MISUSE);
	}
	if (cmd->cmd_da && cmd->cmd_da->items[0]
		&& stat(cmd->cmd_da->items[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_err(cmd->cmd_da->items[0], ": Is a directory");
		child_cleanup_and_exit(shell, cmd, EXIT_CMD_NOT_EXECUTABLE);
	}
	if (!cmd->cmd_da || !cmd->cmd_da->items[0])
		child_cleanup_and_exit(shell, cmd, OK);
}

void	exec_no_pipe(t_shell *sh)
{
	pid_t		child;
	int			status;

	if (sh->cmd->cmd_type != EXTERNAL)
		return (exec_builtin_with_redirs(sh, sh->cmd));
	if (!sh->cmd->cmd_da || sh->cmd->cmd_da->count == 0)
	{
		exec_cleanup_parent(sh->cmd);
		sh->code = 0;
		return ;
	}
	child = fork();
	if (child < 0)
		return (fork_error(sh, sh->cmd));
	if (child == 0)
	{
		exec_child_prep(sh, sh->cmd);
		execve(sh->cmd->cmd_da->items[0], sh->cmd->cmd_da->items, sh->heap_env);
		errno_report(sh->cmd);
		child_cleanup_and_exit(sh, sh->cmd, map_exec_errno_to_exit(errno));
	}
	waitpid_retry(child, &status);
	shell_update_code_from_status(sh, status);
	exec_cleanup_parent(sh->cmd);
}

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
}
