/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_dollar.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:08:43 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 12:16:19 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar_loop(t_lexer *l, size_t *var_len)
{
	while (l->cursor < l->content_len && (ft_isalnum(l->content[l->cursor])
			|| l->content[l->cursor] == '_'))
	{
		(*var_len)++;
		l->cursor++;
	}
}

t_token	lexer_handle_dollar(t_shell *shell, t_lexer *l)
{
	t_token	token;
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*expanded_value;

	l->cursor++;
	var_start = l->cursor;
	var_len = 0;
	if (l->content[l->cursor] == '?')
	{
		var_len = 1;
		l->cursor++;
	}
	else
		handle_dollar_loop(l, &var_len);
	var_name = arena_alloc(sh_work_arena(shell), var_len + 1);
	ft_strlcpy(var_name, &l->content[var_start], var_len + 1);
	expanded_value = expand_dollar_variable(shell, var_name);
	token.type = WORD;
	token.content = expanded_value;
	token.content_len = ft_strlen(expanded_value);
	token.prev = NULL;
	token.next = NULL;
	return (token);
}
