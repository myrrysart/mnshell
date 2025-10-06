/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:54 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/03 18:59:55 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(t_shell *shell, t_cmd_table *cmd)
{
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
