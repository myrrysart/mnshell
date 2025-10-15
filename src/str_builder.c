/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:27:53 by trupham           #+#    #+#             */
/*   Updated: 2025/10/10 12:02:02 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*str_realloc(t_arena *arena, char *src, size_t src_size, size_t new)
{
	char	*str;
	size_t	i;

	str = arena_alloc(arena, new);
	if (!str)
		return (NULL);
	i = 0;
	while (i < src_size)
	{
		str[i] = src[i];
		i++;
	}
	return (str);
}

t_str	*str_init(t_arena *arena, size_t cap)
{
	t_str	*str;

	str = arena_alloc(arena, sizeof(*str));
	if (!str)
		return (NULL);
	str->cap = cap;
	str->len = 0;
	str->str = arena_alloc(arena, cap);
	if (!str->str)
		return (NULL);
	return (str);
}

void	str_append(t_arena *arena, t_str *str, char *item)
{
	int	i;

	i = 0;
	if (!str || !item)
		return ;
	if (str->len + ft_strlen(item) >= str->cap)
	{
		while (str->len + ft_strlen(item) >= str->cap)
			str->cap *= 2;
		str->str = str_realloc(arena, str->str, str->len, str->cap);
		if (!str->str)
			return ;
	}
	while (item[i])
		str->str[str->len++] = item[i++];
	str->str[str->len] = '\0';
}
