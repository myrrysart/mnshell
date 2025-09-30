/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:45:01 by trupham           #+#    #+#             */
/*   Updated: 2025/09/16 13:25:07 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "minishell.h"

t_lexer	build_lexer(char *content)
{
	t_lexer	l;

	l.content = content;
	l.content_len = ft_strlen(content);
	l.cursor = 0;
	l.line = 0;
	return (l);
}

void	trim_left(t_lexer *l)
{
	while (ft_isspace(l->content[l->cursor]) && l->cursor < l->content_len)
		l->cursor++;
}

t_token_type	get_token_type(const char c)
{
	if (c == '$')
		return (DOLLAR);
	if (c == '\'')
		return (SQUOTE);
	if (c == '\"')
		return (DQUOTE);
	if (c == '|')
		return (PIPE);
	if (c == '>')
		return (REDIRECT_OUT);
	if (c == '<')
		return (REDIRECT_IN);
	return (INVALID);
}

bool	is_operator(const char c)
{
	const char	*operators;
	int			i;

	i = 0;
	operators = "|>< \n\t";
	while (operators[i])
	{
		if (c == operators[i])
			return (true);
		i++;
	}
	return (false);
}

t_token	handle_append(t_lexer *l)
{
	t_token	token;

	token.content = &l->content[l->cursor];
	token.type = APPEND;
	token.content_len = 2;
	token.next = NULL;
	token.prev = NULL;
	l->cursor += 2;
	return (token);
}

t_token	handle_heredoc(t_lexer *l)
{
	t_token	token;

	token.content = &l->content[l->cursor];
	token.type = HEREDOC;
	token.content_len = 2;
	token.next = NULL;
	token.prev = NULL;
	l->cursor += 2;
	return (token);
}

t_token	handle_dollar(t_lexer *l)
{
	t_token	token;

	token.content = &l->content[l->cursor];
	token.type = DOLLAR;
	token.content_len = 0;
	token.prev = NULL;
	token.next = NULL;
	while (!ft_isspace(l->content[l->cursor]) && l->cursor < l->content_len)
	{
		token.content_len++;
		l->cursor++;
	}
	return (token);
}

t_token	handle_word(t_lexer *l)
{
	t_token	token;

	token.content_len = 0;
	token.type = WORD;
	token.content = &l->content[l->cursor];
	token.next = NULL;
	token.prev = NULL;
	while (!is_operator(l->content[l->cursor]) && l->cursor < l->content_len)
	{
		token.content_len++;
		l->cursor++;
	}
	return (token);
}

bool	is_quote(const char c)
{
	return (c == '\'' || c == '\"');
}

t_token	handle_squote(t_lexer *l)
{
	t_token	token;

	token.type = SQUOTE;
	token.content = &l->content[l->cursor];
	l->cursor++;
	token.content_len = 1;
	token.prev = NULL;
	token.next = NULL;
	while (l->cursor < l->content_len && l->content[l->cursor] != '\'')
	{
		token.content_len++;
		l->cursor++;
	}
	if (l->cursor < l->content_len && l->content[l->cursor] == '\'')
	{
		token.content_len++;
		l->cursor++;
	}
	else
		token.type = INVALID;
	return (token);
}

t_token	handle_dquote(t_lexer *l)
{
	t_token	token;

	token.type = DQUOTE;
	token.content = &l->content[l->cursor];
	l->cursor++;
	token.content_len = 1;
	token.prev = NULL;
	token.next = NULL;
	while (l->cursor < l->content_len && l->content[l->cursor] != '\"')
	{
		token.content_len++;
		l->cursor++;
	}
	if (l->cursor < l->content_len && l->content[l->cursor] == '\"')
	{
		token.content_len++;
		l->cursor++;
	}
	else
		token.type = INVALID;
	return (token);
}

t_token handle_other_token(t_lexer *l)
{
	t_token	token;

	token = (t_token){};
	token.content_len = 1;
	token.content = &l->content[l->cursor];
	token.type = get_token_type(l->content[l->cursor]);
	token.next = NULL;
	token.prev = NULL;
	l->cursor++;
	return (token);
}
/*@brief: returning a token with a corresponding type, len and content
 * @param: pointer to the lexer structure. this funciton will modify the lexer
 * and moving the cursor forward
 */
t_token	get_next_token(t_lexer *l)
{
	t_token	token;

	ft_bzero(&token, sizeof(t_token));
	if (l->cursor >= l->content_len)
		return (token);
	trim_left(l);
	if (l->content[l->cursor] == '\'')
		return (handle_squote(l));
	if (l->content[l->cursor] == '"')
		return (handle_dquote(l));
	if (l->content[l->cursor] == '>' && l->content[l->cursor + 1] == '>')
		return (handle_append(l));
	if (l->content[l->cursor] == '<' && l->content[l->cursor + 1] == '<')
		return (handle_heredoc(l));
	if (l->content[l->cursor] == '$')
		return (handle_dollar(l));
	if (!is_operator(l->content[l->cursor]) && !is_quote(l->content[l->cursor]))
		return (handle_word(l));
	return (handle_other_token(l));
}

/*@brief: allocate a token node in the memory arena from a copy of token passed in
 *@return: pointer to the allocated token
 */
t_token	*build_token(t_arena *arena, t_token token)
{
	t_token	*new_token;
	size_t	i;

	i = 0;
	new_token = arena_alloc(arena, sizeof(t_token));
	if (!new_token)
		return NULL;
	new_token->content = arena_alloc(arena, token.content_len + 1);
	if (!new_token->content)
		return NULL;
	new_token->content_len = token.content_len;
	new_token->type = token.type;
	while (i < new_token->content_len && token.content[i])
	{
		new_token->content[i] = token.content[i];
		i++;
	}
	new_token->content[i] = '\0';
	return (new_token);
}

/*@brief: build a list of token from the lexer
 *@return: the head of the token list
 */
t_token	*build_token_list(t_arena *arena, t_lexer *l)
{
	t_token	*head;
	t_token	*new;
	t_token	*curr;

	head = build_token(arena, get_next_token(l));
	curr = head;
	if (!head)
		return (NULL);
	while (l->cursor < l->content_len)
	{
		while (curr->next)
			curr = curr->next;
		new = build_token(arena, get_next_token(l));
		if (!new)
			return (NULL);
		curr->next = new;
		new->prev = curr;
	}
	return (head);
}
