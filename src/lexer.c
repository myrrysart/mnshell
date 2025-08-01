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
#include "libft.h"

t_lexer build_lexer(const char *content)
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
	while (isspace(l->content[l->cursor]) && l->cursor < l->content_len)
		l->cursor++;
}

t_token get_next_token(t_lexer *l)
{
	t_token token;

	ft_bzero(&token, sizeof(t_token));
	trim_left(l);
	token.text = &l->content[l->cursor];
	if (l->cursor >= l->content_len)
		return token;
	if (l->content[l->cursor] == '$')
	{
		token.type = DOLLAR;
		token.text_len = 1;
		l->cursor++;
		return token;
	}
	if (l->content[l->cursor] == '\'')
	{
		token.type = SQUOTE;
		token.text_len = 1;
		l->cursor++;
		return token;
	}
	if (l->content[l->cursor] == '\"')
	{
		token.type = DQUOTE;
		token.text_len = 1;
		l->cursor++;
		return token;
	}
	if (l->content[l->cursor] == '|')
	{
		token.type = PIPE;
		token.text_len = 1;
		l->cursor++;
		return token;
	}
	if (l->content[l->cursor] == '>')
	{
		token.type = REDIRECT_OUT;
		token.text_len = 1;
		l->cursor++;
		return token;
	}
	if (l->content[l->cursor] == '<')
	{
		token.type = REDIRECT_IN;
		token.text_len = 1;
		l->cursor++;
		return token;
	}
	if (ft_isalpha(l->content[l->cursor]))
	{
		token.type = CMD;
		while (ft_isalpha(l->content[l->cursor]))
		{
			token.text_len += 1;
			l->cursor++;
		}
		return token;
	}
	token.type = INVALID;
	token.text_len = 1;
	l->cursor++;
	return token;
}
