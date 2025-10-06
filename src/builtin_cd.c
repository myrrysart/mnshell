/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:44 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/06 12:15:53 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_error(t_shell *shell, char *path, int cd_error_code)
{
	if (cd_error_code == 1)
	{
		ft_printf("cd: too many arguments\n");
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	else if (cd_error_code == 2)
	{
		ft_printf("cd: HOME not set\n");
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	else if (cd_error_code == 3)
	{
		ft_printf("cd: %s: No such file or directory\n", path);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	else if (cd_error_code == 4)
	{
		ft_printf("cd: error retrieving current directory\n");
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
}

static void	cd_go_to_home(t_shell *shell, char *path, char *home)
{
	home = get_env_var(shell, "HOME");
	if (!home)
		return (cd_error(shell, path, 2));
	path = home;
}

void	builtin_cd(t_shell *shell, t_cmd_table *cmd)
{
	char	*path;
	char	*home;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	path = NULL;
	home = NULL;
	if (cmd->cmd_da->items[2])
		return (cd_error(shell, path, 1));
	ft_strlcpy(old_pwd, shell->working_directory, PATH_MAX);
	if (!cmd->cmd_da->items[1])
		cd_go_to_home(shell, path, home);
	else
		path = cmd->cmd_da->items[1];
	if (chdir(path) != 0)
		return (cd_error(shell, path, 3));
	if (getcwd(new_pwd, PATH_MAX))
	{
		ft_strlcpy(shell->working_directory, new_pwd, PATH_MAX);
		update_pwd_env(shell, old_pwd, new_pwd);
	}
	else
		return (cd_error(shell, path, 4));
	shell->code = OK;
}
