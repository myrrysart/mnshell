/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:30:38 by jyniemit          #+#    #+#             */
/*   Updated: 2025/07/29 14:30:57 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_received_signal = 0;

void signal_handler(int sig)
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

void init_signals(t_shell *shell)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, &shell->saved_sigint);
	sigaction(SIGQUIT, &sa, &shell->saved_sigquit);
	sigaction(SIGTERM, &sa, &shell->saved_sigterm);
}

void handle_signal(t_shell *shell, int sig)
{
	if (sig == SIGINT)
	{
		shell->code = EXIT_SIGINT;
	}
	g_received_signal = 0;
}

