/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:37:33 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 11:44:22 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_env(char **env, t_shell *shell)
{
	int	i;

	i = -1;
	shell->original_env = env;
	shell->env_count = 0;
	while (env[shell->env_count])
		shell->env_count++;
	if (shell->env_count > 0)
		shell->env_capacity = shell->env_count * 2;
	else
		shell->env_capacity = 8;
	shell->heap_env = (char **)arena_alloc(shell->arena, sizeof(char *)
			* (shell->env_capacity + 1));
	if (!shell->heap_env)
		return ;
	while (++i < shell->env_count)
		shell->heap_env[i] = arena_strdup(shell->arena, env[i]);
	shell->heap_env[shell->env_count] = NULL;
}

void	init_shell(char **av, char **env, t_shell *shell)
{
	shell->arena = arena_init(ARENA_CAP);
	if (!shell->arena)
	{
		shell->code = EXIT_SHELLINITFAIL;
		shell->state |= SHOULD_EXIT;
		return ;
	}
	shell->pipeline = arena_alloc(shell->arena, sizeof(*shell->pipeline));
	if (shell->pipeline)
		shell->pipeline->tmp_fd = -1;
	init_env(env, shell);
	if (!getcwd(shell->working_directory, PATH_MAX))
	{
		shell->code = EXIT_SHELLINITFAIL;
		shell->state |= SHOULD_EXIT;
	}
	init_shell_env(shell, av);
	if (!isatty(STDIN_FILENO))
		shell->state |= NON_INTERACTIVE;
	shell->heredoc_index = -1;
}
