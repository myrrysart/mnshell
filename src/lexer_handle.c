/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:46:57 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 12:09:00 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	lexer_handle_append(t_lexer *l)
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

t_token	lexer_handle_heredoc(t_lexer *l)
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

t_token	lexer_handle_word(t_lexer *l, bool wp)
{
	t_token	token;

	token.wp = wp;
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

t_token	lexer_handle_other_token(t_lexer *l)
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
