/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:13:15 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 01:55:11 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	enter_heredoc(t_shell *sh, struct sigaction *old_int)
{
	struct sigaction	sa;

	sh->state |= IN_HEREDOC;
	sigaction(SIGINT, NULL, old_int);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	leave_heredoc(t_shell *sh, struct sigaction *old_int)
{
	sh->state &= ~IN_HEREDOC;
	sigaction(SIGINT, old_int, NULL);
	sig_mode_set(SIGMODE_EVAL);
}
