/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:48:11 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 11:51:29 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	is_quote(const char c)
{
	return (c == '\'' || c == '\"');
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
