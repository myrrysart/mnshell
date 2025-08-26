/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/25 17:13:06 by jyniemit         ###   ########.fr       */
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
	shell->env_capacity = shell->env_count + 10;
	shell->heap_env = (char **)s_malloc(sizeof(char *) * (shell->env_capacity + 1));
	while (++i < shell->env_count)
		shell->heap_env[i] = a_strdup(env[i]);
	shell->heap_env[shell->env_count] = NULL;
}

void	init_shell(int ac, char **av, char **env, t_shell *shell)
{
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
	init_env(env, shell);
	if (!getcwd(shell->working_directory, PATH_MAX))
	{
		shell->code = EXIT_SHELLINITFAIL;
		shell->state |= SHOULD_EXIT;
	}
}

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
	// t_lexer l = build_lexer(shell->command_line);
	// t_token *t = build_token_list(&l);
	// while (t)
	// {
	// 	printf("(%s) token length: %d\t\t\t%.*s\n",print_token_type(t->type), (int)t->text_len, (int)t->text_len, t->text);
	// 	t = t->next;
	// }
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
