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

static void	wait_and_reap_others(int to_reap, int *status)
{
	while (to_reap > 0)
	{
		if (waitpid(-1, status, 0) == -1)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		to_reap--;
	}
}

static void	handle_sigint_status(t_shell *shell, int status)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		clean_sig_pipe(shell);
		sh_abort(shell, EXIT_SIGINT);
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
	wait_and_reap_others(to_reap, &status);
	shell_update_code_from_status(shell, status);
	handle_sigint_status(shell, status);
	if (!(shell->state & EVALUATING))
		clean_sig_pipe(shell);
}
