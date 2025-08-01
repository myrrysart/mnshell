/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:50:29 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/18 15:44:46 by jyniemit         ###   ########.fr       */
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

int		init_env(t_shell *shell)
{
	extern char	**environ;
	int			i;
	
	i = -1;
	shell->original_env = environ;
	while (environ[shell->env_count])
		shell->env_count++;
	shell->capacity = shell->env_count + 10;
	shell->heap_env = malloc(sizeof(char *) * (shell->capacity + 1))
	if(!shell->heap_env)
		return (0);
	while (++i < shell->env_count)
	{
		shell->heap_env[i] = ft_strdup(environ[i]);
		if (!shell->heap_env)
		{
			while (--i >= 0)
				free(shell->heap_env);
			free(shell->heap_env);
			shell->heap_env = NULL;
			return (0);
		}
	}
	shell->heap_env[shell->env_count] = NULL;
	return (1);
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
