/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:44 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 15:42:23 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_shell *shell)
{
	char	*path;
	char	*home;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!shell)
		return (EXIT_BUILTIN_MISUSE);
	if (shell->args[2])
	{
		ft_printf("cd: too many arguments\n");
		return (EXIT_GENERAL_ERROR);
	}
	ft_strlcpy(old_pwd, shell->working_directory, PATH_MAX);
	if (!shell->args[1])
	{
		home = get_env_var(shell, "HOME");
		if (!home)
		{
			ft_printf("cd: HOME not set\n");
			return (EXIT_GENERAL_ERROR);
		}
		path = home;
	}
	else
		path = shell->args[1];
	if (chdir(path) != 0)
	{
		ft_printf("cd: %s: No such file or directory\n", path);
		return (EXIT_GENERAL_ERROR);
	}
	if (getcwd(new_pwd, PATH_MAX))
	{
		ft_strlcpy(shell->working_directory, new_pwd, PATH_MAX);
		update_pwd_env(shell, old_pwd, new_pwd);
	}
	else
	{
		ft_printf("cd: error retrieving current directory\n");
		return (EXIT_GENERAL_ERROR);
	}
	return (OK);
}
