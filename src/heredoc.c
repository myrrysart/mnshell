/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/23 17:59:39 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int read_heredoc(t_shell *shell)
{
    char tmp_name[32];
    char *line;
    char *delimiter;
    int fd;
	int	heredoc_index;

	heredoc_index = shell->heredoc_index;
	delimiter = shell->heredoc_delim[heredoc_index];
    ft_strlcpy(tmp_name, "/tmp/ms_", sizeof(tmp_name));
    tmp_name[8] = '0' + heredoc_index;
    tmp_name[9] = '\0';
    fd = open(tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd == -1) return -1;
    while ((line = readline("> "))) 
	{
		if (!line)
			break;
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
            && ft_strlen(line) == ft_strlen(delimiter)) 
		{
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    fd = open(tmp_name, O_RDONLY);
    unlink(tmp_name);
    return fd;
}
