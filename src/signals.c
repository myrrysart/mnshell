/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:30:38 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 22:13:28 by jyniemit         ###   ########.fr       */
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
