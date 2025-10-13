/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:54 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/09 11:23:34 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(t_shell *shell, t_cmd_table *cmd)
{
	char	*arg;
	int		i;

	arg = cmd->cmd_da->items[1];
	if (cmd->cmd_da->items[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	if (arg)
	{
		i = 0;
		if (arg[i] == '+' || arg[i] == '-')
			i++;
		while (arg[i])
		{
			if (!ft_isdigit(arg[i]))
			{
				ft_putstr_fd("exit: ", 2);
				ft_putstr_fd(arg, 2);
				ft_putendl_fd(": numeric argument required", 2);
				shell->code = EXIT_BUILTIN_MISUSE;
				shell->state |= SHOULD_EXIT;
				if (!(shell->state & HAS_PIPE))
					write(STDOUT_FILENO, "exit\n", 5);
				return ;
			}
			i++;
		}
		shell->code = (unsigned char)ft_atoi(arg);
	}
	shell->state |= SHOULD_EXIT;
	if (!(shell->state & HAS_PIPE) && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
}
