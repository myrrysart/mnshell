/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:50:29 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/19 12:19:36 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av)
{
	t_shell	shell;

	shell = (t_shell){};
	init_signals(&shell);
	init_shell(ac, av, &shell);
	run_shell(&shell);
	return (shell.code);
}
	/*TODO: On ac and av: need to figure out if child process shells need these
	* things, ortesting or something else. Now I'm just casting them to the
	* void, but keeping them until we know for sure*/
void	init_shell(int ac, char **av, t_shell *shell)
{
	(void)ac;
	(void)av;
	shell->input_fd = STDIN_FILENO;
	shell->output_fd = STDOUT_FILENO;
	shell->error_fd = STDERR_FILENO;
	shell->pipe_read_fd = -1;
	shell->pipe_write_fd = -1;
	if (!getcwd(shell->working_directory, PATH_MAX) || !init_env(shell))
	{
		shell->code = EXIT_SHELLINITFAIL;
		shell->state |= SHOULD_EXIT;
	}
}

