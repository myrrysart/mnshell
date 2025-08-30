/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:23:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/30 18:32:16 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_shell *shell)
{
	if (!shell->args[0])
		return (OK);
	if (ft_strncmp(shell->args[0], "echo", 5) == 0)
        return builtin_echo(shell->args);
	else if (ft_strncmp(shell->args[0], "exit", 5) == 0)
		return builtin_exit(shell);
//	else if (ft_strncmp(shell->args[0], "export", 7) == 0)
//        return (set_env_var(shell->args,));
	else if (ft_strncmp(shell->args[0], "env", 4) == 0)
        return (print_env(shell));
	return (EXIT_CMD_NOT_FOUND);
}
