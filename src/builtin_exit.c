/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:54 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/08 12:50:33 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(t_shell *shell, t_cmd_table *cmd)
{
	int	i;

	i = -1;
	if (cmd->cmd_da->items[1])
		while (cmd->cmd_da->items[1][++i])
		{
			if (!(ft_isdigit(cmd->cmd_da->items[1][i])))
			{
				ft_printf("Exit:%s: numeric argument required\n", cmd->cmd_da->items[1]);
				return ;
			}
		}
	if (cmd->cmd_da->items[2])
	{
		ft_printf("Exit: too many arguments.\n");
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	shell->state |= SHOULD_EXIT;
	if (cmd->cmd_da->items[1])
		shell->code = ft_atoi(cmd->cmd_da->items[1]);
	printf("exit\n");
}
