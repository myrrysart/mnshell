/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:53:40 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/10 11:28:31 by trupham          ###   ########.fr       */
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

static char	*rm_quote(t_shell *sh, t_token *tok)
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
	t_str	*str;

	str = str_init(sh_work_arena(sh), STR_CAP);
	if (!str)
		return false;
	if (*first && cmd->cmd_type == EXTERNAL)
	{
		str_append(sh_work_arena(sh), str, exec_copy_bin_path(sh, rm_quote(sh, *tok)));
		da_append(sh_work_arena(sh), cmd->cmd_da, str->str);
		*tok = (*tok)->next;
	}
	else if (!(*first) && (*tok)->wp && (*tok)->next && !((*tok)->next->wp))
	{
		while (*tok && (*tok)->next && !((*tok)->next->wp))
		{
			str_append(sh_work_arena(sh), str, rm_quote(sh, *tok));
			*tok = (*tok)->next;
		}
		if (!((*tok)->next))
		{
			str_append(sh_work_arena(sh), str, rm_quote(sh, *tok));
			*tok = (*tok)->next;
		}
		da_append(sh_work_arena(sh), cmd->cmd_da, str->str);
	}
	else
	{
		str_append(sh_work_arena(sh), str, rm_quote(sh, *tok));
		da_append(sh_work_arena(sh), cmd->cmd_da, str->str);
		*tok = (*tok)->next;
	}
	*first = 0;
	return (true);
}
