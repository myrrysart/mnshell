/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 00:56:22 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 01:02:04 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_flag(t_shell *shell, t_token *t)
{
	while (t)
	{
		if (t->type == PIPE)
			shell->state |= HAS_PIPE;
		if (t->type == SQUOTE || t->type == DQUOTE)
			shell->state |= HAS_QUOTE;
		if (t->type == REDIRECT_IN || t->type == HEREDOC)
			shell->state |= HAS_INPUT_REDIR;
		if (t->type == REDIRECT_OUT || t->type == APPEND)
			shell->state |= HAS_OUTPUT_REDIR;
		if (t->type == HEREDOC)
			shell->heredoc_count++;
		t = t->next;
	}
}

int	reset_signal(t_shell *shell)
{
	if (g_received_signal)
	{
		handle_signal(shell, g_received_signal);
		if (shell->state & SHOULD_EXIT)
			return (1);
	}
	return (0);
}
