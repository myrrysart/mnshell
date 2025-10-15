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

static bool	check_last_token(t_token *tok)
{
	while (tok->next)
		tok = tok->next;
	if (tok->type == PIPE || tok->type == REDIRECT_OUT
		|| tok->type == REDIRECT_IN || tok->type == APPEND
		|| tok->type == HEREDOC)
		return (write(2, "Syntax error: missing expression\n", 33), false);
	return (true);
}

/* checking for PIPE, REDIRECTION and APPEND syntax
 */
bool	parser_is_syntax_correct(t_token *token)
{
	if (!check_last_token(token))
		return (false);
	while (token && token->next)
	{
		if (token->type == INVALID)
			return (write(2, "Syntax error: invalid token\n", 28), false);
		if (token->type == PIPE && (token->next->type != WORD || !token->prev))
			return (write(2, "Syntax error: invalid pipe\n", 27), false);
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN
				|| token->type == APPEND || token->type == HEREDOC)
			&& (token->next->type != WORD && token->next->type != SQUOTE
				&& token->next->type != DQUOTE))
			return (write(2, "Syntax error: invalid redirect\n", 31), false);
		token = token->next;
	}
	if (token && token->type == INVALID)
		return (write(2, "Syntax error: invalid token\n", 28), false);
	return (true);
}
