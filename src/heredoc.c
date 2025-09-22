/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/22 15:00:22 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		read_heredoc(t_shell *shell)
{
	char	*line;
	int		interrupted;
	int		read_fd;

	shell->state |= IN_HEREDOC;
	interrupted = 0;
	g_received_signal = 0;
	while (!interrupted)
	{
		line = readline("> ");
		if (g_received_signal == SIGINT)
		{
			interrupted = 1;
			g_received_signal = 0;
			break;
		}
		if (!line)
			break;
		if (ft_strncmp(line, shell->heredoc_delim[shell->, ft_strlen(line) + 1) == 0)
		{
			free(line);
			break;
		}
		write(shell->heredoc_fd, line, ft_strlen(line));
		write(shell->heredoc_fd, "\n", 1);
		free(line);
	}
	shell->state &= ~IN_HEREDOC;
	if (interrupted)
	{
		close(shell->heredoc_fd);
		shell->heredoc_fd = -1;
		return (-1);
	}
	return (0);
}
