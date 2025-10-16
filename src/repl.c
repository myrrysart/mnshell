/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 01:09:15 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (sh_abort(shell, EXIT_PARSE_ERROR));
	shell->cmd = parser_cmd_build_many(shell, t);
	if (!shell->cmd)
		shell->state &= ~(HAS_PIPE | EVALUATING);
	if (!shell->cmd)
		return ;
	if (shell->cmd && shell->state & HAS_PIPE)
		exec_pipeline(shell);
	else
		exec_no_pipe(shell);
	if (shell->cmd)
		close_cmd_fds(shell->cmd);
	reset_flags(shell);
}

static void	process_command_line(t_shell *shell, char *line)
{
	add_history(line);
	ft_strlcpy(shell->command_line, line, ARG_MAX - 1);
	shell->command_line[ARG_MAX - 1] = '\0';
	shell->state |= EVALUATING;
	set_eval_signal_mode(shell);
	shell_begin_frame(shell);
	parse_and_execute(shell);
	shell_end_frame(shell);
	set_prompt_signal_mode(shell);
}

void	run_shell(t_shell *shell)
{
	char	*line;
	char	*raw_line;

	while (!(shell->state & SHOULD_EXIT))
	{
		if (reset_signal(shell))
			break ;
		set_prompt_signal_mode(shell);
		line = readline(PROMPT);
		if (reset_signal(shell))
		{
			free(line);
			break ;
		}
		if (!line)
		{
			close_cmd_fds(shell->cmd);
			write(STDOUT_FILENO, "exit\n", 5);
			shell->state |= SHOULD_EXIT;
			break ;
		}
		if (*line)
			process_command_line(shell, line);
		free(line);
	}
}
