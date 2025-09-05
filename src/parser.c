/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:27:57 by trupham           #+#    #+#             */
/*   Updated: 2025/08/25 15:50:59 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "arena.h"
#include "lexer.h"
#include <stdio.h>

/* checking for PIPE, REDIRECTION and APPEND syntax
 */
bool	parser_is_syntax_correct(t_token *token)
{
	t_token	*last;

	if (!token)
		return (false);
	last = token;
	while (last->next)
		last = last->next;
	if (last->type == PIPE || last->type == REDIRECT_OUT
		|| last->type == REDIRECT_IN || last->type == APPEND)
		return (false);
	while (token && token->next)
	{
		if (token->type == PIPE && token->next->type != WORD)
			return false;
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN
		|| token->type == APPEND)
			&& token->next->type != WORD)
			return false;
		if (token->type == PIPE && !token->prev)
			return false;
		token = token->next;
	}
	return (true);
}

t_cmd_table *parser_cmd_build_one(t_arena *arena, t_token *token)
{
	t_cmd_table *new_cmd;

	new_cmd = arena_alloc(arena, sizeof(*new_cmd));
	if (!new_cmd)
		return NULL;
	new_cmd->cmd_da = da_cmd_init(arena, DA_CAP);
	if (!new_cmd->cmd_da)
		return NULL;
	while (token && token->type != PIPE)
	{
		da_append(arena, new_cmd->cmd_da, token->text);
		token = token->next;
	}
	return new_cmd;
}

static bool parser_cmd_build_curr(t_arena *arena, t_cmd_table **curr, t_token *token)
{
	t_cmd_table *new_cmd;

	new_cmd = parser_cmd_build_one(arena, token);
	if (!new_cmd)
		return false;
	(*curr)->next = new_cmd;
	new_cmd->prev = *curr;
	return true;
}

t_cmd_table *parser_cmd_build_many(t_arena *arena, t_token *token)
{
	t_cmd_table *cmd_table_head;
	t_cmd_table *curr;
	t_token *token_end;

	if (!token)
		return NULL;
	cmd_table_head = parser_cmd_build_one(arena, token);
	if (!cmd_table_head)
		return NULL;
	while (token && token->type != PIPE)
		token = token->next;
	curr = cmd_table_head;
	while (token && token->next)
	{
		token_end = token->next;
		while (curr->next)
			curr = curr->next;
		while (token_end && token_end->type != PIPE)
			token_end = token_end->next;
		if (!token_end)
		{
			if (!parser_cmd_build_curr(arena, &curr, token->next))
				return NULL;
			break;
		}
		else if (token_end && token_end->type == PIPE)
		{
			if (!parser_cmd_build_curr(arena, &curr, token->next))
				return NULL;
			token = token_end;
		}
	}
	return cmd_table_head;
}

/* realloc the buf pointer but in the arena
 */
void *ft_realloc(t_arena *arena, char **src, size_t src_size, size_t new_size)
{
	char **new_ptr;
	size_t i;

	if (new_size == 0)
		return NULL;
	new_ptr = arena_alloc(arena, sizeof(*new_ptr) * new_size);
	if (!new_ptr)
		return NULL;
	i = 0;
	while (i < src_size)
	{
		new_ptr[i] = arena_strdup(arena, src[i]);
		i++;
	}
	return new_ptr;
}

/* initialize the dynamic array for the command arguments
 */
t_da *da_cmd_init(t_arena *arena, size_t cap)
{
	t_da *da;

	da = arena_alloc(arena, sizeof(*da));
	if (!da)
		return NULL;
	da->cap = cap;
	da->count = 0;
	da->items = arena_alloc(arena, sizeof(*(da->items)) * cap);
	if (!da->items)
		return NULL;
	return da;
}

void da_append(t_arena *arena, t_da *da, char *item)
{
	if (da->count + 1 >= da->cap)
	{
		while (da->count + 1 >= da->cap)
			da->cap *= 2;
		da->items = ft_realloc(arena, da->items, da->count + 1, da->cap);
	}
	da->items[da->count++] = arena_strdup(arena, item);
}
