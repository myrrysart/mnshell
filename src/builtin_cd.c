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

void	builtin_cd(t_shell *shell, t_cmd_table *cmd)
{
	char	*path;
	char	*home;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	(void)cmd;
	if (!shell)
	{
		shell->code = EXIT_BUILTIN_MISUSE;
		return;
	}
	if (cmd->cmd_da->items[2])
	{
		ft_printf("cd: too many arguments\n");
		shell->code = EXIT_GENERAL_ERROR;
		return;
	}
	ft_strlcpy(old_pwd, shell->working_directory, PATH_MAX);
	if (!cmd->cmd_da->items[1])
	{
		home = get_env_var(shell, "HOME");
		if (!home)
		{
			ft_printf("cd: HOME not set\n");
			shell->code = EXIT_GENERAL_ERROR;
			return;
		}
		path = home;
	}
	else
		path = cmd->cmd_da->items[1];
	if (chdir(path) != 0)
	{
		ft_printf("cd: %s: No such file or directory\n", path);
		shell->code = EXIT_GENERAL_ERROR;
		return;
	}
	if (getcwd(new_pwd, PATH_MAX))
	{
		ft_strlcpy(shell->working_directory, new_pwd, PATH_MAX);
		update_pwd_env(shell, old_pwd, new_pwd);
	}
	else
	{
		ft_printf("cd: error retrieving current directory\n");
		shell->code = EXIT_GENERAL_ERROR;
		return;
	}
	shell->code = OK;
}
