/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/16 16:38:06 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_heredoc_temp_file(int *fd)
{
	char	temp_path[256];
	char	*num_str;
	char	prefix[25];
	static int	temp_counter = 0;

	ft_strlcpy(prefix, "/tmp/minishell_heredoc_", sizeof(prefix));
	num_str = ft_itoa(temp_counter++);
	ft_strlcpy(temp_path, prefix, sizeof(temp_path));
	ft_strlcat(temp_path, num_str, sizeof(temp_path));
	free(num_str);
	*fd = open(temp_path, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (*fd == -1)
		return (-1);
	unlink(temp_path);
	return (0);
}

int		read_heredoc(char *delim, int *fd, t_shell *shell)
{
	char	*line;
	int		interrupted;

	shell->state |= IN_HEREDOC;
	if (*fd == -1)
	{
		if (create_heredoc_temp_file(fd) == -1)
			return (-1);
	}
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
		if (ft_strncmp(line, delim, ft_strlen(line) + 1) == 0)
		{
			free(line);
			break;
		}
		write(*fd, line, ft_strlen(line));
		write(*fd, "\n", 1);
		free(line);
	}
	shell->state &= ~IN_HEREDOC;
	if (interrupted)
	{
		close(*fd);
		*fd = -1;
		return (-1);
	}
	return (0);
}
