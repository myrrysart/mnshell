/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_others.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:23:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 04:11:40 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_error(t_shell *shell, t_cmd_table *cmd)
{
	perror("fork");
	shell->code = errno;
	exec_cleanup_parent(cmd);
}

static void	exec_guard(t_shell *sh)
{
	exec_cleanup_parent(sh->cmd);
	sh->code = 0;
	return ;
}

void	exec_no_pipe(t_shell *sh)
{
	pid_t		child;
	int			status;

	if (sh->cmd->cmd_type != EXTERNAL)
		return (exec_builtin_with_redirs(sh, sh->cmd));
	if (!sh->cmd->cmd_da || sh->cmd->cmd_da->count == 0)
		exec_guard(sh);
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
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT
		&& isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	exec_cleanup_parent(sh->cmd);
}
