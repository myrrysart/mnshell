/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_da.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:36:51 by trupham           #+#    #+#             */
/*   Updated: 2025/10/10 11:28:31 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* realloc the buf pointer but in the arena
 */
static void	*ft_realloc(t_arena *arena, char **src, size_t src_size,
		size_t new_size)
{
	char	**new_ptr;
	size_t	i;

	if (new_size == 0)
		return (NULL);
	new_ptr = arena_alloc(arena, sizeof(*new_ptr) * (new_size + 1));
	if (!new_ptr)
		return (NULL);
	i = 0;
	while (i < src_size)
	{
		new_ptr[i] = arena_strdup(arena, src[i]);
		i++;
	}
	new_ptr[i] = NULL;
	return (new_ptr);
}

/* initialize the dynamic array for the command arguments
 */
t_da	*da_cmd_init(t_arena *arena, size_t cap)
{
	t_da	*da;

	da = arena_alloc(arena, sizeof(*da));
	if (!da)
		return (NULL);
	da->cap = cap;
	da->count = 0;
	da->items = arena_alloc(arena, sizeof(*(da->items)) * (cap + 1));
	if (!da->items)
		return (NULL);
	da->items[0] = NULL;
	return (da);
}

void	da_append(t_arena *arena, t_da *da, char *item)
{
	if (da->count + 1 >= da->cap)
	{
		while (da->count + 1 >= da->cap)
			da->cap *= 2;
		da->items = ft_realloc(arena, da->items, da->count + 1, da->cap);
	}
	da->items[da->count] = arena_strdup(arena, item);
	da->count++;
	da->items[da->count] = NULL;
}
