/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:42:05 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/16 16:52:24 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pwd(t_shell *shell, t_cmd_table *cmd)
{
	char	current_dir[PATH_MAX];

	(void)cmd;
	if (getcwd(current_dir, PATH_MAX))
	{
		ft_printf("%s\n", current_dir);
		ft_strlcpy(shell->working_directory, current_dir, PATH_MAX);
	}
	else if (shell->working_directory[0])
		ft_printf("%s\n", shell->working_directory);
	else
	{
		ft_putendl_fd("pwd: cannot determine current directory", STDERR_FILENO);
		shell->code = EXIT_GENERAL_ERROR;
		return ;
	}
	shell->code = OK;
}

void	builtin_select(t_shell *shell, t_cmd_table *cmd)
{
	const t_builtin	builtin_table[BUILTIN_COUNT] = {
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
