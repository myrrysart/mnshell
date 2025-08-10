/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_arena.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 11:38:02 by trupham           #+#    #+#             */
/*   Updated: 2025/08/07 11:38:35 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "arena.h"

void print_arena()
{
	t_arena *arena = get_static_arena();

	int count = 0;
	while (arena)
	{
		ft_printf("[debug] arena cursor %d: %d\n", count, arena->cursor);
		count++;
		arena = arena->next;
	}
}

int main()
{
	t_arena *arena = get_static_arena();

	char *buf = s_malloc(12);
	ft_strlcpy(buf, "hello world", 12);
	print_arena();
	t_list *list = s_malloc(sizeof(t_list));
	(void)list;
	print_arena();
	char *bur1 = s_malloc(1000);
	(void)bur1;
	print_arena();
	char *bur2 = s_malloc(14);
	(void)bur2;
	print_arena();
	arena_free(arena);
}
