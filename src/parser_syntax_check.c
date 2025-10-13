/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:31:29 by trupham           #+#    #+#             */
/*   Updated: 2025/10/13 20:55:21 by jyniemit         ###   ########.fr       */
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

static bool	check_last_token(t_token *tok)
{
	while (tok->next)
		tok = tok->next;
	if (tok->type == PIPE || tok->type == REDIRECT_OUT
		|| tok->type == REDIRECT_IN || tok->type == APPEND
		|| tok->type == HEREDOC)
		return (write(2, "Syntax error: missing expression\n", 34), false);
	return (true);
}

/* checking for PIPE, REDIRECTION and APPEND syntax
 */
bool	parser_is_syntax_correct(t_token *token)
{
	if (!token)
		return (write(2, "Syntax error: trailing whitespace\n", 35), false);
	if (!parser_is_quote_count_correct(token))
		return (write(2, "Syntax error: unclosed quotes\n", 31), false);
	if (!check_last_token(token))
		return (false);
	while (token && token->next)
	{
		if (token->type == INVALID)
			return (write(2, "Syntax error: invalid token\n", 30), false);
		if (token->type == PIPE && (token->next->type != WORD || !token->prev))
			return (write(2, "Syntax error: invalid pipe\n", 28), false);
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN
				|| token->type == APPEND || token->type == HEREDOC)
			&& (token->next->type != WORD && token->next->type != SQUOTE
				&& token->next->type != DQUOTE))
			return (write(2, "Syntax error: invalid redirect\n", 32), false);
		token = token->next;
	}
	if (token && token->type == INVALID)
		return (write(2, "Syntax error: invalid token\n", 30), false);
	return (true);
}
