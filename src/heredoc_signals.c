/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:13:15 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/07 16:50:10 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	enter_heredoc(t_shell *sh, struct sigaction *old_int)
{
	struct sigaction	ign;

	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	ign.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ign, old_int);
	sh->state |= IN_HEREDOC;
}

void	leave_heredoc(t_shell *sh, struct sigaction *old_int)
{
	sh->state &= ~IN_HEREDOC;
	sigaction(SIGINT, old_int, NULL);
}
