/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_setups.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:18:42 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/06 13:30:20 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	heredoc_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_received_signal = SIGINT;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_DFL);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}

void	init_signals(t_shell *shell)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, &shell->saved_sigint);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, &shell->saved_sigquit);
	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGTERM, &sa, &shell->saved_sigterm);
}
