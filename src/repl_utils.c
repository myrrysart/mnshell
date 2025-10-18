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

void	reset_flags(t_shell *shell)
{
	shell->state &= ~(HAS_PIPE | EVALUATING | HAS_QUOTE
			| HAS_INPUT_REDIR | HAS_OUTPUT_REDIR);
}

void	process_command_line(t_shell *shell, char *line)
{
	if (!(shell->state & NON_INTERACTIVE))
		add_history(line);
	ft_strlcpy(shell->command_line, line, ARG_MAX - 1);
	shell->command_line[ARG_MAX - 1] = '\0';
	shell->state |= EVALUATING;
	sig_mode_set(SIGMODE_EVAL);
	shell_begin_frame(shell);
	repl_parse_and_execute(shell);
	shell_end_frame(shell);
	sig_mode_set(SIGMODE_PROMPT);
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
