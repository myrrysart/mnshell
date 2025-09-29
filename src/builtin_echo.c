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

void	builtin_echo(t_shell *shell, t_cmd_table *cmd)
{
	int	i;

	i = 1;
	while (cmd->cmd_da->items[i])
	{
		ft_printf("%s", cmd->cmd_da->items[i]);
		if (cmd->cmd_da->items[i + 1])
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
	shell->code  = OK;
}
