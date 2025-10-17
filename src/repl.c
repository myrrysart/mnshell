/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 17:06:38 by jyniemit         ###   ########.fr       */
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

static void	reset_flags(t_shell *shell)
{
	shell->state &= ~(HAS_PIPE | EVALUATING | HAS_QUOTE
			| HAS_INPUT_REDIR | HAS_OUTPUT_REDIR);
}

static void	parse_and_execute(t_shell *shell)
{
	t_lexer	l;
	t_token	*t;

	if (!(shell->state & EVALUATING) || (shell->state & SHOULD_EXIT))
		return ;
	if (ft_strlen(shell->command_line) == 0)
		return ;
	l = build_lexer(shell->command_line);
	t = build_token_list(shell, &l);
	check_flag(shell, t);
	if (!parser_is_syntax_correct(t))
		return (shell_abort_eval(shell, EXIT_PARSE_ERROR));
	shell->cmd = parser_cmd_build_many(shell, t);
	if (!shell->cmd)
		shell->state &= ~(HAS_PIPE | EVALUATING);
	if (!shell->cmd)
		return ;
	if (shell->cmd && shell->state & HAS_PIPE)
		exec_pipeline(shell);
	else
		exec_no_pipe(shell);
	reset_flags(shell);
}

static void	process_command_line(t_shell *shell, char *line)
{
	add_history(line);
	ft_strlcpy(shell->command_line, line, ARG_MAX - 1);
	shell->command_line[ARG_MAX - 1] = '\0';
	shell->state |= EVALUATING;
	shell_begin_frame(shell);
	parse_and_execute(shell);
	if (shell->cmd)
		close_all_cmd_fds(shell->cmd);
	shell_end_frame(shell);
}

void	run_shell(t_shell *shell)
{
	char	*line;

	while (!(shell->state & SHOULD_EXIT))
	{
		if (g_received_signal)
			handle_signal(shell, g_received_signal);
		line = readline(PROMPT);
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			shell->state |= SHOULD_EXIT;
			break ;
		}
		if (*line)
			process_command_line(shell, line);
		free(line);
	}
}
