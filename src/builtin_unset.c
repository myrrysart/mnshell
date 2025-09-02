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

int	builtin_unset(t_shell *shell)
{
	int	i;

	if (!shell)
		return (EXIT_BUILTIN_MISUSE);
	if (!shell->args[1])
		return (OK);
	i = 0;
	while (shell->args[++i])
		unset_env_var(shell, shell->args[i]);
	return (OK);
}
