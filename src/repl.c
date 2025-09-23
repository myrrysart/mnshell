/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/23 18:19:10 by jyniemit         ###   ########.fr       */
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
	shell->env_capacity = shell->env_count * 2;
	shell->heap_env = (char **)arena_alloc(shell->arena, sizeof(char *)
			* (shell->env_capacity + 1));
	if (!shell->heap_env)
		return ;
	while (++i < shell->env_count)
		shell->heap_env[i] = arena_strdup(shell->arena, env[i]);
	shell->heap_env[shell->env_count] = NULL;
}

void	init_shell(int ac, char **av, char **env, t_shell *shell)
{
	(void)ac;
	(void)av;
	shell->arena = arena_init(ARENA_CAP);
	if (!shell->arena)
	{
		shell->code = EXIT_SHELLINITFAIL;
		shell->state |= SHOULD_EXIT;
		return ;
	}
	shell->pipeline = arena_alloc(shell->arena, sizeof(*shell->pipeline));
	init_env(env, shell);
	init_shell_env(shell, av);
	if (!getcwd(shell->working_directory, PATH_MAX))
	{
		shell->code = EXIT_SHELLINITFAIL;
		shell->state |= SHOULD_EXIT;
	}
	shell->heredoc_index = -1;
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
	t = build_token_list(&l);
	if (!parser_is_syntax_correct(t))
	{
		ft_printf("[debug] Syntax Error\n");
		return ;
	}
	shell->cmd = parser_cmd_build_many(shell, t);
	if(shell->cmd)
		pipeline(shell);
	shell->state &= ~EVALUATING;
}

void	run_shell(t_shell *shell)
{
	char	*line;
	char	*raw_line;

	while (!(shell->state & SHOULD_EXIT))
	{
		if (g_received_signal)
			handle_signal(shell, g_received_signal);
		if (isatty(fileno(stdin)))
			line = readline("minishell$ ");
		else
		{
			raw_line = get_next_line(fileno(stdin));
			if (!raw_line)
			{
				shell->state |= SHOULD_EXIT;
				break ;
			}
			line = ft_strtrim(raw_line, "\n");
			free(raw_line);
		}
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
