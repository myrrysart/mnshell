/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_wait.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 03:13:07 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/21 11:45:25 by jyniemit         ###   ########.fr       */
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

static int    wait_all_and_get_last(pid_t last_child, int count)
{
    int   status;
    int   last_status;
    int   remaining;
    pid_t pid;

	status = 0;
	last_status = 0;
    remaining = count;
    while (remaining > 0)
    {
        pid = waitpid(-1, &status, 0);
        if (pid == -1)
        {
            if (errno == EINTR)
                continue;
            break;
        }
        if (pid == last_child)
            last_status = status;
        remaining--;
    }
    return last_status;
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

	if (count == 0)
		return ;
	status = wait_all_and_get_last(last_child, count);
	shell_update_code_from_status(shell, status);
	handle_sigint_status(shell, status);
	if (!(shell->state & EVALUATING))
		clean_sig_pipe(shell);
}
