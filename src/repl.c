/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/07/29 14:50:17 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_shell(int ac, char **av, t_shell *shell)
{
	/*TODO: need to figure out if child process shells need these things, or
	 * testing or something else. Now I'm just casting them to the void, but
	 * keeping them until we know for sure*/
	(void)ac;
	(void)av;
	
	shell->state = 0;
	shell->code = OK;
	shell->last_code = OK;
	shell->argc = 0;
	shell->input_fd = STDIN_FILENO;
	shell->output_fd = STDOUT_FILENO;
	shell->error_fd = STDERR_FILENO;
	shell->pipe_read_fd = -1;
	shell->pipe_write_fd = -1;
	shell->token_count = 0;
	if (getcwd(shell->working_directory, PATH_MAX) == NULL)
	{
		shell->code = EXIT_SHELLINITFAIL;
		shell->state |= SHOULD_EXIT;
	}
}

static void parse_and_execute(t_shell *shell)
{
	if (!(shell->state & EVALUATING))
		return ;
	if (ft_strlen(shell->command_line) == 0)
		return ;
	if (ft_strncmp(shell->command_line, "exit", 5) == 0)
	{
		shell->state |= SHOULD_EXIT;
		return ;
	}
	/* TODO: parsing goes here. */
	printf("Command received: %s\n", shell->command_line);
	shell->code = OK;
	shell->state &= ~EVALUATING;
}

void run_shell(t_shell *shell)
{
	char *line;

	while (!(shell->state & SHOULD_EXIT))
	{
		if (g_received_signal)
			handle_signal(shell, g_received_signal);
		line = readline("minishell$ ");
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			shell->state |= SHOULD_EXIT;
			break;
		}
		if (*line)
		{
			add_history(line);
			ft_strlcpy(shell->command_line, line, ARG_MAX - 1);
			shell->command_line[ARG_MAX - 1] = '\0';
			shell->state |= EVALUATING;
		}
		parse_and_execute(shell);
		free(line);
	}
}
