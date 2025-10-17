/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:54 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 15:55:09 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	builtin_exit_error(t_shell *shell, char *arg, int code)
{
	if (code == 1)
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
	if (code == 2)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		shell->code = EXIT_GENERAL_ERROR;
	}
}

void	builtin_exit(t_shell *shell, t_cmd_table *cmd)
{
	char		*arg;
	int			i;
	long long	j;

	arg = cmd->cmd_da->items[1];
	if (arg)
	{
		i = 0;
		if (arg[i] == '+' || arg[i] == '-')
			i++;
		while (arg[i])
		{
			if (!ft_isdigit(arg[i]))
			{
				builtin_exit_error(shell, arg, 1);
				return ;
			}
			i++;
		}
		if (cmd->cmd_da->items[2])
		{
			builtin_exit_error(shell, arg, 2);
			return ;
		}
		j = ft_atoll(arg);
		if (j < 0)
		{
			builtin_exit_error(shell, arg, 1);
			return ;
		}
		shell->code = (unsigned char)j;
	}
	shell->state |= SHOULD_EXIT;
	if (!(shell->state & HAS_PIPE) && isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
}
