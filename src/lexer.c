/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:45:01 by trupham           #+#    #+#             */
/*   Updated: 2025/07/31 11:52:32 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "arena.h"
#include "libft.h"

t_lexer build_lexer(char *content)
{
	t_lexer l;

	l.content = content;
	l.content_len = ft_strlen(content);
	l.cursor = 0;
	l.line = 0;
	return l;
}

void trim_left(t_lexer *l)
{
	while (ft_isspace(l->content[l->cursor]) && l->cursor < l->content_len)
		l->cursor++;
}

t_token_type get_token_type(const char c)
{
	if (c == '$')
		return DOLLAR;
	if (c == '\'')
		return SQUOTE;
	if (c == '\"')
		return DQUOTE;
	if (c == '|')
		return PIPE;
	if (c == '>')
		return REDIRECT_OUT;
	if (c == '<')
		return REDIRECT_IN;
	return INVALID;
}

bool is_operator(const char c)
{
	const char *operators;
	int i;

	i = 0;
	operators = "\'\"|><$";
	while (operators[i])
	{
		if (c == operators[i])
			return true;
		i++;
	}
	return false;
}

t_token handle_append(t_lexer *l)
{
	t_token token;

	ft_bzero(&token, sizeof(t_token));
	token.text = &l->content[l->cursor];
	token.type = APPEND;
	token.text_len = 2;
	l->cursor += 2;
	return token;
}

t_token handle_squote(t_lexer *l)
{
	t_token token;

	ft_bzero(&token, sizeof(t_token));
	token.type = SQUOTE;
	while (l->content[l->cursor])
	{
		// TODO:
	}
	return token;
}

t_token handle_word(t_lexer *l)
{
	t_token token;

	token.type = WORD;
	token.text = &l->content[l->cursor];
	while (!is_operator(l->content[l->cursor]))
	{
		token.text_len += 1;
		l->cursor++;
	}
	return token;
}

/*@brief: returning a token with a corresponding type, len and content
 * @param: pointer to the lexer structure. this funciton will modify the lexer
 * and moving the cursor forward
 */
t_token get_next_token(t_lexer *l)
{
	t_token token;
	t_token_type type;

	ft_bzero(&token, sizeof(t_token));
	if (l->content[l->cursor] != '\'' && l->content[l->cursor] != '\"')
		trim_left(l);
	token.text = &l->content[l->cursor];
	if (l->cursor >= l->content_len)
		return token;
	type = get_token_type(l->content[l->cursor]);
	if (type == REDIRECT_OUT && l->content[l->cursor + 1] == '>')
		return handle_append(l);
	if (!is_operator(l->content[l->cursor]))
		return handle_word(l);
	token.type = type;
	token.text_len = 1;
	l->cursor++;
	return token;
}

/*@brief: allocate a token node in the memory arena from a copy of token passed in
 *@return: pointer to the allocated token
 */
t_token *build_token(t_token token)
{
	t_token *new_token;
	size_t i;

	i = 0;
	new_token = s_malloc(sizeof(t_token));
	new_token->text = s_malloc(token.text_len);
	new_token->text_len = token.text_len;
	new_token->type = token.type;
	while (i < new_token->text_len && token.text[i])
	{
		new_token->text[i] = token.text[i];
		i++;
	}
	new_token->text[i] = '\0';
	return new_token;
}

/*@brief: build a list of token from the lexer
 *@return: the head of the token list
 */
t_token *build_token_list(t_lexer *l)
{
	t_token *head;
	t_token *new;
	t_token *curr;

	head = build_token(get_next_token(l));
	curr = head;
	if (!head)
		return NULL;
	while (l->cursor < l->content_len)
	{
		while (curr->next)
			curr = curr->next;
		new = build_token(get_next_token(l));
		if (!new)
			return NULL;
		curr->next = new;
		new->prev = curr;
	}
	return head;
}
