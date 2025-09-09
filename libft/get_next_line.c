/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:40:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/09 17:06:26 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	safe_free(void **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

static int	process_newline(char **remains, char **line, char *newline_pos)
{
	char	*temp;

	*line = gnl_substr(*remains, 0, newline_pos - *remains + 1);
	if (!*line)
	{
		safe_free((void *)remains);
		return (0);
	}
	if (*(newline_pos + 1))
	{
		temp = gnl_strdup(newline_pos + 1);
		if (!temp)
		{
			safe_free((void *)remains);
			safe_free((void *)line);
			return (0);
		}
		safe_free((void *)remains);
		*remains = temp;
	}
	else
		safe_free((void *)remains);
	return (1);
}

static char	*extract_line(char **remains)
{
	char	*line;
	char	*newline_pos;

	if (!remains || !*remains)
		return (NULL);
	newline_pos = gnl_strchr(*remains, '\n');
	if (!newline_pos)
	{
		line = gnl_strdup(*remains);
		safe_free((void *)remains);
		return (line);
	}
	if (!process_newline(remains, &line, newline_pos))
		return (NULL);
	return (line);
}

static int	read_to_remains(char **remains, int fd)
{
	int		bytes_read;
	int		continue_reading;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (-1);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	continue_reading = 1;
	while (bytes_read > 0 && continue_reading)
	{
		buffer[bytes_read] = '\0';
		*remains = concat_buffer_to_remains(*remains, buffer);
		if (!*remains)
		{
			safe_free((void *)&buffer);
			return (-1);
		}
		if (gnl_strchr(*remains, '\n'))
			continue_reading = 0;
		else
			bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	safe_free((void *)&buffer);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*remains;
	char		*line;
	int			bytes_read;

	if (BUFFER_SIZE <= 0 || fd < 0)
	{
		safe_free((void *)&remains);
		return (NULL);
	}
	if (remains && gnl_strchr(remains, '\n'))
		return (extract_line(&remains));
	bytes_read = read_to_remains(&remains, fd);
	if (bytes_read < 0)
	{
		safe_free((void *)&remains);
		return (NULL);
	}
	if (remains && *remains)
	{
		line = extract_line(&remains);
		return (line);
	}
	return (NULL);
}
