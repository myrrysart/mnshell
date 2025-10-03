/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:50:29 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/03 11:23:24 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)ac;
	shell = (t_shell){};
	init_shell(av, env, &shell);
	init_signals(&shell);
	run_shell(&shell);
	arena_free(shell.arena);
	rl_clear_history();
	return (shell.code);
}
