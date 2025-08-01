/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/01 19:11:43 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

	/* TODO: parsing goes here. Currently just calling a printf to tell
	 * what has been written on the command line*/
static void	parse_and_execute(t_shell *shell)
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
	printf("Command received: %s\n", shell->command_line);
	shell->code = OK;
	shell->state &= ~EVALUATING;
}

void	run_shell(t_shell *shell)
{
	char	*line;

	while (!(shell->state & SHOULD_EXIT))
	{
		if (g_received_signal)
			handle_signal(shell, g_received_signal);
		line = readline("minishell$ ");
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			shell->state |= SHOULD_EXIT;
			break ;
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
