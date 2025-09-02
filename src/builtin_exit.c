/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:54 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 15:52:05 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_shell *shell)
{
	if (shell->args[2])
	{
		ft_printf("Exit: too many arguments.\n");
		return (EXIT_GENERAL_ERROR);
	}
	shell->state |= SHOULD_EXIT;
	if (shell->args[1])
		return (ft_atoi(shell->args[1]));
	return (OK);
}
