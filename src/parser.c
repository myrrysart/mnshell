/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:27:57 by trupham           #+#    #+#             */
/*   Updated: 2025/08/25 15:50:59 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool parser_is_valid_redirect(t_token *token)
{
	if (!token)
		return false;
	while (token && token->next)
	{
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN)
			&& token->next->type != WORD)
			return false;
		token = token->next;
	}
	return true;
}

bool parser_is_valid_pipe(t_token *token)
{
	if (!token)
		return false;
	while (token && token->next)
	{
		if (token->type == PIPE && token->next->type != WORD)
			return false;
		token = token->next;
	}
	return true;
}

bool parser_syntax_check(t_token *token)
{
	if (!parser_is_valid_pipe(token))
		return false;
	if (!parser_is_valid_redirect(token))
		return false;
	return true;
}
