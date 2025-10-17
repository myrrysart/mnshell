/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:50 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 15:42:23 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	has_nl_flag(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	else
		return (false);
	if (str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static void	init_echo(int *i, bool *print_nl, bool *echo)
{
	*i = 1;
	*print_nl = true;
	*echo = false;
}

void	builtin_echo(t_shell *shell, t_cmd_table *cmd)
{
	int		i;
	bool	print_nl;
	bool	echo;

	init_echo(&i, &print_nl, &echo);
	while (cmd->cmd_da->items[i])
	{
		if (has_nl_flag(cmd->cmd_da->items[i]) && !echo)
		{
			print_nl = false;
			i++;
			continue ;
		}
		else
			echo = true;
		if (echo)
			ft_putstr_fd(cmd->cmd_da->items[i], cmd->fd_out);
		if (cmd->cmd_da->items[i + 1])
			ft_putstr_fd(" ", cmd->fd_out);
		i++;
	}
	if (print_nl)
		ft_putstr_fd("\n", cmd->fd_out);
	shell->code = OK;
}
