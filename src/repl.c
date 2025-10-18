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

void	repl_parse_and_execute(t_shell *shell)
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
	reset_flags(shell);
}

static int	repl_step_non_interactive(t_shell *shell)
{
	char	*line;
	size_t	len;

	sig_mode_set(SIGMODE_PROMPT);
	line = get_next_line(STDIN_FILENO);
	if (reset_signal(shell))
		return (free(line), 0);
	if (!line)
		return (shell->state |= SHOULD_EXIT, 0);
	if (*line)
	{
		len = ft_strlen(line);
		if (len && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (*line)
			process_command_line(shell, line);
	}
	free(line);
	return (1);
}

static int	repl_step_interactive(t_shell *shell)
{
	char	*line;

	sig_mode_set(SIGMODE_PROMPT);
	line = readline(PROMPT);
	if (reset_signal(shell))
		return (free(line), 0);
	if (!line)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		shell->state |= SHOULD_EXIT;
		return (0);
	}
	if (*line)
		process_command_line(shell, line);
	free(line);
	return (1);
}

void	run_shell(t_shell *shell)
{
	while (!(shell->state & SHOULD_EXIT))
	{
		if (reset_signal(shell))
			break ;
		if (shell->state & NON_INTERACTIVE)
		{
			if (!repl_step_non_interactive(shell))
				break ;
		}
		else if (!repl_step_interactive(shell))
			break ;
	}
}
