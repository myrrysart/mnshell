/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:30:38 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/06 13:30:23 by jyniemit         ###   ########.fr       */
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
		shell->code = EXIT_SIGINT;
	else if (sig == SIGTERM)
		shell->code = EXIT_SIGTERM;
	g_received_signal = 0;
}
