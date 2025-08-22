/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:28:11 by trupham           #+#    #+#             */
/*   Updated: 2025/08/03 11:54:22 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
#define ARENA_H
#include <stdint.h>
#include <stdlib.h>

#ifndef ARENA_CAP
#define ARENA_CAP 1024
#endif // !ARENA_CAP
typedef struct s_arena
{
	uint64_t capacity;
	uint64_t cursor;
	unsigned char *data;
	struct s_arena *next;
} t_arena;

t_arena *arena_init(uint64_t capacity);
void *arena_alloc(t_arena *arena, uint64_t size);
void *s_malloc(uint64_t size);
void arena_free(t_arena *arena);
t_arena *get_static_arena();

#endif // !ARENA_H
