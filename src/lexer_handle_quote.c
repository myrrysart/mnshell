/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:04:36 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 12:04:51 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	lexer_handle_squote(t_lexer *l)
{
	t_token	token;

	token.type = SQUOTE;
	token.content = &l->content[l->cursor];
	l->cursor++;
	token.content_len = 1;
	token.prev = NULL;
	token.next = NULL;
	while (l->cursor < l->content_len && !is_operator(l->content[l->cursor]))
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

t_token	lexer_handle_dquote(t_lexer *l)
{
	t_token	token;

	token.type = DQUOTE;
	token.content = &l->content[l->cursor];
	l->cursor++;
	token.content_len = 1;
	token.prev = NULL;
	token.next = NULL;
	while (l->cursor < l->content_len && !is_operator(l->content[l->cursor]))
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
