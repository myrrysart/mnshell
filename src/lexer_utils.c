/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:48:11 by trupham           #+#    #+#             */
/*   Updated: 2025/10/07 12:50:54 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator(const char c)
{
	const char	*operators;
	int			i;

	i = 0;
	operators = "'\"|>< \n\t";
	while (operators[i])
	{
		if (c == operators[i])
			return (true);
		i++;
	}
	return (false);
}

bool	is_quote(const char c)
{
	return (c == '\'' || c == '\"');
}

bool	trim_left(t_lexer *l)
{
	size_t	count;

	count = 0;
	while (ft_isspace(l->content[l->cursor]) && l->cursor < l->content_len)
	{
		count++;
		l->cursor++;
	}
	return (count > 0);
}

t_token_type	get_token_type(const char c)
{
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
