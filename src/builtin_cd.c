/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:44 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/14 19:24:43 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_error(t_shell *shell, char *path, int code)
{
	if (code == 1)
	{
		ft_putendl_fd("cd: too many arguments", STDERR_FILENO);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	if (code == 2)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	if (code == 3)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	if (code == 4)
	{
		ft_putendl_fd("cd: error retrieving current directory", STDERR_FILENO);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	if (code == 5)
	{
		ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
}
static int	set_target_path(t_shell *shell, t_cmd_table *cmd, char **out)
{
	char	*arg;

	arg = cmd->cmd_da->items[1];
	if (!arg)
	{
		*out = get_env_var(shell, "HOME");
		if (!*out)
			return (cd_error(shell, NULL, 2), 0);
		return (1);
	}
	if (!ft_strncmp(arg, "-", 2))
	{
		*out = get_env_var(shell, "OLDPWD");
		if (!*out)
			return (cd_error(shell, NULL, 5), 0);
		ft_printf("%s\n", *out);
		return (1);
	}
	*out = arg;
	return (1);
}
void	builtin_cd(t_shell *shell, t_cmd_table *cmd)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	path = NULL;
	if (cmd->cmd_da->items[2])
		return (cd_error(shell, NULL, 1));
	ft_strlcpy(old_pwd, shell->working_directory, PATH_MAX);
	if (!set_target_path(shell, cmd, &path))
		return ;
	if (chdir(path) != 0)
		return (cd_error(shell, path, 3));
	if (!getcwd(new_pwd, PATH_MAX))
		return (cd_error(shell, path, 4));
	ft_strlcpy(shell->working_directory, new_pwd, PATH_MAX);
	update_pwd_env(shell, old_pwd, new_pwd);
	shell->code = OK;
}
