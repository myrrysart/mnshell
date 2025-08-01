/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:50:29 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/18 15:33:43 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	main(int ac, char **av)
{
	t_shell	shell;

	init_signals(&shell);
	ft_bzero(&shell, sizeof(t_shell));
	init_shell(ac, av, &shell);
	run_shell(&shell);
	return (shell.code);
}

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
