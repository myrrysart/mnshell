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

void	builtin_select(t_shell *shell, t_cmd_table *cmd)
{
	const builtin	builtin_table[BUILTIN_COUNT] = {
		builtin_cd,
		builtin_echo,
		builtin_exit,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
	};

	builtin_table[cmd->cmd_type](shell, cmd);
}
