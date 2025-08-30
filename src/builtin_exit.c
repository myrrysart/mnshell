/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 18:23:36 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/30 18:35:52 by jyniemit         ###   ########.fr       */
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
	if (shell->args[1])
		exit(ft_atoi(shell->args[1]));
	exit(OK);
}
