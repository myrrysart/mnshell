/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_wait.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 03:13:07 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 03:13:11 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_sig_pipe(t_shell *shell)
{
	if (shell->pipeline && shell->pipeline->tmp_fd != -1)
	{
		close(shell->pipeline->tmp_fd);
		shell->pipeline->tmp_fd = -1;
	}
}

void	exec_pipeline_wait_and_finalize(t_shell *shell, pid_t last_child,
		int count)
{
	int	status;
	int	to_reap;

	if (count == 0)
		return ;
	waitpid_retry(last_child, &status);
	to_reap = count - 1;
	while (to_reap > 0)
	{
		if (waitpid(-1, &status, 0) == -1)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		to_reap--;
	}
	shell_update_code_from_status(shell, status);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		clean_sig_pipe(shell);
		sh_abort(shell, EXIT_SIGINT);
	}
	if (!(shell->state & EVALUATING))
		clean_sig_pipe(shell);
}
