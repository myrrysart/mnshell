/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:42:05 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/03 19:04:07 by jyniemit         ###   ########.fr       */
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
		ft_printf("pwd: cannot determine current directory\n");
		shell->code = EXIT_GENERAL_ERROR;
		return;
	}
	shell->code = OK;
}
