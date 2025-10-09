/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_frame.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:09:47 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/09 11:34:11 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arena	*sh_work_arena(t_shell *sh)
{
	if (sh->active_arena)
		return (sh->active_arena);
	return (sh->arena);
}

void	shell_begin_frame(t_shell *sh)
{
	if (sh->frame_arena)
		arena_free(sh->frame_arena);
	sh->frame_arena = arena_init(ARENA_CAP);
	if (!sh->frame_arena)
	{
		sh->code = EXIT_SHELLINITFAIL;
		return ;
	}
	sh->active_arena = sh->frame_arena;
	sh->heredoc_index = -1;
	sh->heredoc_count = 0;
}

void	shell_end_frame(t_shell *sh)
{
	if (sh->frame_arena)
		arena_free(sh->frame_arena);
	sh->frame_arena = NULL;
	sh->active_arena = NULL;
	sh->state &= ~(HAS_PIPE | IN_SQUOTE | IN_DQUOTE | EVALUATING
			| HEREDOC_EXPAND);
	sh->heredoc_index = -1;
	sh->heredoc_count = 0;
}
