/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:30:38 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/06 13:21:45 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

volatile sig_atomic_t	g_received_signal = 0;

void	signal_handler(int sig)
{
	g_received_signal = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

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
		shell->code = EXIT_SIGINT;
	else if (sig == SIGTERM)
		shell->code = EXIT_SIGTERM;
	g_received_signal = 0;
}
