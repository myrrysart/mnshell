/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:42:10 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 15:42:23 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_unset(t_shell *shell, t_cmd_table *cmd)
{
	int	i;

	(void)cmd;
	if (!shell)
	{
		shell->code = EXIT_BUILTIN_MISUSE;
		return ;
	}
	if (!cmd->cmd_da->items[1])
	{
		shell->code = OK;
		return ;
	}
	i = 0;
	while (cmd->cmd_da->items[++i])
		unset_env_var(shell, cmd->cmd_da->items[i]);
	shell->code = OK;
}
