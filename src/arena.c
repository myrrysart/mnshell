/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:54:34 by trupham           #+#    #+#             */
/*   Updated: 2025/10/10 11:39:59 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*@brief: initialize the memory arena with max size of capacity
 * @return: pointer to the arena
 */
t_arena	*arena_init(size_t capacity)
{
	t_arena	*arena;

	arena = ft_calloc(1, sizeof(t_arena));
	if (!arena)
		return (NULL);
	arena->data = ft_calloc(capacity, sizeof(unsigned char));
	if (!arena->data)
	{
		free(arena);
		return (NULL);
	}
	arena->cursor = 0;
	arena->capacity = capacity;
	arena->next = NULL;
	return (arena);
}

/* @brief: allocate memory in the arena. if there is not enough space, move to
 * the next avaiable arena in the linked list
 * @params: the head arena, the size to be allocated
 * @return: pointer to the buffer in the arena
 */
void	*arena_alloc(t_arena *arena, size_t size)
{
	void	*buf;
	t_arena	*next;

	next = NULL;
	if (size > arena->capacity)
		return NULL;
	while (arena->cursor + size > arena->capacity)
	{
		if (arena->next == NULL)
		{
			next = arena_init(arena->capacity);
			if (!next)
				return (NULL);
			arena->next = next;
		}
		arena = arena->next;
	}
	buf = &arena->data[arena->cursor];
	arena->cursor += size;
	return (buf);
}

void	arena_free(t_arena *arena)
{
	t_arena	*next;
	t_arena	*current;

	current = arena;
	next = NULL;
	while (current)
	{
		next = current->next;
		free(current->data);
		current->data = NULL;
		free(current);
		current = next;
	}
	arena = NULL;
}

char	*arena_strdup(t_arena *arena, const char *s)
{
	size_t	len;
	char	*dup;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = arena_alloc(arena, len + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}
