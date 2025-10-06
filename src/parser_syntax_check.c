/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:31:29 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 15:31:43 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	parser_is_quote_count_correct(t_token *token)
{
	int		i;
	size_t	count_squote;
	size_t	count_dquote;

	count_squote = 0;
	count_dquote = 0;
	while (token)
	{
		i = 0;
		while (token->content[i])
		{
			if (token->content[i] == '"')
				count_dquote++;
			if (token->content[i] == '\'')
				count_squote++;
			i++;
		}
		token = token->next;
	}
	if (count_dquote % 2 != 0 || count_squote % 2 != 0)
		return (false);
	return (true);
}

/* checking for PIPE, REDIRECTION and APPEND syntax
 */
bool	parser_is_syntax_correct(t_token *token)
{
	t_token	*last;

	if (!token || !parser_is_quote_count_correct(token))
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
			return (false);
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN
				|| token->type == APPEND || token->type == HEREDOC)
			&& (token->next->type != WORD && token->next->type != SQUOTE
				&& token->next->type != DQUOTE))
			return (false);
		if (token->type == PIPE && !token->prev)
			return (false);
		token = token->next;
	}
	return (true);
}
