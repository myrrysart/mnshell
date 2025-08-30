/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:40:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/30 14:55:04 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void safe_free(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

char *get_next_line(int fd)
{
	static char *remains = NULL;
	char *buffer;
	char *line;
	int bytes_read;

	bytes_read = 0;
	buffer = malloc(GNL_BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (fd < 0 || GNL_BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		if (remains)
			safe_free((void *)&remains);
		safe_free((void *)&buffer);
		return (NULL);
	}
	if (!remains)
		remains = gnl_strdup("");
	while (!gnl_strchr(remains, '\n') && 
		(bytes_read = read(fd, buffer, GNL_BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';  
		remains = gnl_strjoin(remains, buffer);
	}
	if (bytes_read < 0)
	{
		safe_free((void *)&remains);
		safe_free((void *)&buffer);
		return (NULL);
	}
	
	line = extract_line(&remains);
	if (!line && !remains)
	{
		safe_free((void *)&buffer);
		return (NULL);
	}
	safe_free((void *)&buffer);
	return (line);
}
