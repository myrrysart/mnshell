/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:53:40 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/07 18:59:53 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*unquoate_token(t_shell *sh, t_token *tok)
{
	char	*inner;
	size_t	len;

	if (tok->content_len < 2)
		return (arena_strdup(sh_work_arena(sh), ""));
	len = tok->content_len - 2;
	inner = arena_alloc(sh_work_arena(sh), len + 1);
	if (!inner)
		return (NULL);
	ft_memcpy(inner, tok->content + 1, len);
	inner[len] = '\0';
	return (inner);
}

static char	*build_arg_from_token(t_shell *sh, t_token *tok)
{
	char	*inner;

	if (tok->type == SQUOTE)
	{
		inner = unquoate_token(sh, tok);
		return (inner);
	}
	if (tok->type == DQUOTE)
	{
		inner = unquoate_token(sh, tok);
		return (parser_expand_dollar(sh, inner));
	}
	return (parser_expand_dollar(sh, tok->content));
}

bool	handle_word(t_shell *sh, t_cmd_table *cmd, t_token **tok, int *first)
{
	char	*arg;

	arg = build_arg_from_token(sh, *tok);
	if (*first && cmd->cmd_type == EXTERNAL)
		arg = exec_copy_bin_path(sh, arg);
	if (arg)
		da_append(sh_work_arena(sh), cmd->cmd_da, arg);
	*first = 0;
	*tok = (*tok)->next;
	return (true);
}
