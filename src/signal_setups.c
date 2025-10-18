/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_setups.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:18:42 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 01:41:29 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

void	init_signals(t_shell *shell)
{
	sigaction(SIGINT, NULL, &shell->saved_sigint);
	sigaction(SIGQUIT, NULL, &shell->saved_sigquit);
	sigaction(SIGTERM, NULL, &shell->saved_sigterm);
	sig_mode_set(SIGMODE_PROMPT);
}
