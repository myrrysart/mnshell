/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 01:59:23 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 02:10:42 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sh_sig_eval(int sig)
{
	g_received_signal = sig;
}

static void	set_action(int sig, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags = flags;
	sigaction(sig, &sa, NULL);
}

static void	set_signal_child_mode(void)
{
	set_action(SIGINT, SIG_DFL, 0);
	set_action(SIGQUIT, SIG_DFL, 0);
	set_action(SIGTERM, SIG_DFL, 0);
}

static void	set_prompt_mode(void)
{
	set_action(SIGINT, sh_sig_prompt, SA_RESTART);
	set_action(SIGQUIT, SIG_IGN, 0);
	set_action(SIGTERM, sh_sig_eval, SA_RESTART);
}

void	sig_mode_set(t_sig_mode mode)
{
	if (mode == SIGMODE_CHILD)
		set_signal_child_mode();
	else if (mode == SIGMODE_HEREDOC_CHILD)
	{
		set_action(SIGINT, sh_sig_hdoc_child, 0);
		set_action(SIGQUIT, SIG_IGN, 0);
		set_action(SIGTERM, SIG_DFL, 0);
	}
	else if (mode == SIGMODE_EVAL)
	{
		set_action(SIGINT, sh_sig_eval, SA_RESTART);
		set_action(SIGQUIT, SIG_IGN, 0);
		set_action(SIGTERM, sh_sig_eval, SA_RESTART);
	}
	else
		set_prompt_mode();
}
