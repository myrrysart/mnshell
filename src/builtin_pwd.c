/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:42:05 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 15:42:23 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	builtin_pwd(t_shell *shell)
{
	char	current_dir[PATH_MAX];

	if (!shell)
		return (EXIT_BUILTIN_MISUSE);
	if (getcwd(current_dir, PATH_MAX))
	{
		ft_printf("%s\n", current_dir);
		ft_strlcpy(shell->working_directory, current_dir, PATH_MAX);
	}
	else
	{
		if (shell->working_directory[0])
			ft_printf("%s\n", shell->working_directory);
		else
		{
			perror("pwd");
			return (EXIT_GENERAL_ERROR);
		}
	}
	return (OK);
}
