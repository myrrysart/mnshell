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

/* checking for PIPE and REDIRECTION syntax
 */
bool parser_is_syntax_correct(t_token *token)
{
	t_token *last;

	if (!token)
		return false;
	last = token;
	while (last->next)
		last = last->next;
	if (last->type == PIPE || last->type == REDIRECT_OUT
		|| last->type == REDIRECT_IN || last->type == APPEND)
		return false;
	while (token && token->next)
	{
		if (token->type == PIPE && token->next->type != WORD)
			return false;
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN 
		|| token->type == APPEND)
			&& token->next->type != WORD)
			return false;
		token = token->next;
	}
	return true;
}
