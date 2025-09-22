/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:27:57 by trupham           #+#    #+#             */
/*   Updated: 2025/09/23 14:24:03 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		|| last->type == REDIRECT_IN || last->type == APPEND
		|| last->type == HEREDOC)
		return (false);
	while (token && token->next)
	{
		if (token->type == PIPE && token->next->type != WORD)
			return false;
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN
		|| token->type == APPEND || token->type == HEREDOC)
			&& token->next->type != WORD)
			return false;
		if (token->type == PIPE && !token->prev)
			return false;
		token = token->next;
	}
	return (true);
}

t_cmd_table *parser_cmd_build_one(t_shell *shell, t_token *token)
{
	t_cmd_table *new_cmd;
	int			i;

	new_cmd = arena_alloc(shell->arena, sizeof(*new_cmd));
	i = -1;
	if (!new_cmd)
		return NULL;
	new_cmd->cmd_da = da_cmd_init(shell->arena, DA_CAP);
	new_cmd->cmd_pos = MID;
	if (!new_cmd->cmd_da)
		return NULL;
	while (token && token->type != PIPE)
	{
		if (token->type == REDIRECT_OUT)
		{
			new_cmd->fd_out = open(token->next->content, O_WRONLY | O_CREAT, 0644);
			if (new_cmd->fd_out == -1)
			{
				shell->code = EXIT_REDIRECT_ERROR;
				return (NULL);
			}
			token = token->next->next;
		}
		else if (token->type == REDIRECT_IN)
		{
			new_cmd->fd_in = open(token->next->content, O_RDONLY);
			if (new_cmd->fd_in == -1)
			{
				shell->code = EXIT_REDIRECT_ERROR;
				return (NULL);
			}
			token = token->next->next;
		}
		else if (token->type == APPEND)
		{
			new_cmd->fd_out = open(token->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (new_cmd->fd_out == -1)
			{
				shell->code = EXIT_REDIRECT_ERROR;
				return (NULL);
			}
			token = token->next->next;
		}
		else if (token->type == HEREDOC)
		{
			shell->heredoc_count++;
			while (token->next->content[++i])
				shell->heredoc_delim[shell->heredoc_count][i] = token->next->content[i];
			new_cmd->heredoc_index = shell->heredoc_count;
			token = token->next->next;
		}
		else
		{
			char *cmd = exec_copy_bin_path(shell, token->content);
			da_append(shell->arena, new_cmd->cmd_da, cmd);
			token = token->next;
		}
	}
	return new_cmd;
}

static bool parser_cmd_build_curr(t_shell *shell, t_cmd_table **curr, t_token *token)
{
	t_cmd_table *new_cmd;

	new_cmd = parser_cmd_build_one(shell, token);
	if (!new_cmd)
		return false;
	(*curr)->next = new_cmd;
	new_cmd->prev = *curr;
	return true;
}

static bool parser_cmd_build_main(t_shell *shell, t_cmd_table *head, t_token *token)
{
	t_token *token_end;
	t_cmd_table *curr;

	curr = head;
	while (token && token->next)
	{
		token_end = token->next;
		while (curr->next)
			curr = curr->next;
		while (token_end && token_end->type != PIPE)
			token_end = token_end->next;
		if (!token_end)
		{
			if (!parser_cmd_build_curr(shell, &curr, token->next))
				return false;
			break;
		}
		else if (token_end && token_end->type == PIPE)
		{
			if (!parser_cmd_build_curr(shell, &curr, token->next))
				return false;
			token = token_end;
		}
	}
	return true;
}

t_cmd_table *parser_cmd_build_many(t_shell *shell, t_token *token)
{
	t_cmd_table *cmd_table_head;

	if (!token)
		return NULL;
	cmd_table_head = parser_cmd_build_one(shell, token);
	if (!cmd_table_head)
		return NULL;
	cmd_table_head->cmd_pos = FIRST;
	while (token && token->type != PIPE)
		token = token->next;
	if (!parser_cmd_build_main(shell, cmd_table_head, token))
		return NULL;
	t_cmd_table *curr;
	curr = cmd_table_head;
	while (curr->next)
		curr = curr->next;
	curr->cmd_pos = LAST;
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
