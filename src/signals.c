/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:30:38 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 02:10:44 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	waitpid_retry(pid_t pid, int *st)
{
	int	i;

	i = waitpid(pid, st, 0);
	while ((i == -1 && errno == EINTR))
		i = waitpid(pid, st, 0);
	return (i);
}

void	handle_signal(t_shell *shell, int sig)
{
	if (sig == SIGINT)
	{
		if (shell->state & EVALUATING)
		{
			write(STDOUT_FILENO, "\n", 1);
			sh_abort(shell, EXIT_SIGINT);
			shell->code = EXIT_SIGINT;
		}
		else
			shell->code = EXIT_SIGINT;
	}
	else if (sig == SIGTERM)
	{
		if (shell->state & EVALUATING)
			sh_abort(shell, OK);
		shell->code = OK;
		shell->state |= SHOULD_EXIT;
	}
	g_received_signal = 0;
}

void	sh_sig_hdoc_child(int sig)
{
	g_received_signal = sig;
	if (sig == SIGINT)
		close(STDIN_FILENO);
}

void	sh_sig_prompt(int sig)
{
	g_received_signal = sig;
	if (sig == SIGINT && isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
