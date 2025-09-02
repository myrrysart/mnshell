/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:23:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 16:05:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_shell *shell)
{
	int	result;

	if (!shell->args[0])
		return (OK);
	update_last_command(shell, shell->args[0]);
	if (ft_strncmp(shell->args[0], "echo", 5) == 0)
		result = builtin_echo(shell->args);
	else if (ft_strncmp(shell->args[0], "exit", 5) == 0)
		result = builtin_exit(shell);
	else if (ft_strncmp(shell->args[0], "pwd", 4) == 0)
		result = builtin_pwd(shell);
	else if (ft_strncmp(shell->args[0], "cd", 3) == 0)
		result = builtin_cd(shell);
	else if (ft_strncmp(shell->args[0], "export", 7) == 0)
		result = builtin_export(shell);
	else if (ft_strncmp(shell->args[0], "unset", 6) == 0)
		result = builtin_unset(shell);
	else if (ft_strncmp(shell->args[0], "env", 4) == 0)
		result = print_env(shell);
	else
		result = EXIT_CMD_NOT_FOUND;
	return (result);
}
