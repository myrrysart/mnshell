/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:42:14 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 15:42:23 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell_env(t_shell *shell, char **av)
{
	char	*current_shlvl;
	int		shlvl;
	char	*new_shlvl;

	set_env_var(shell, "SHELL", av[0]);
	current_shlvl = get_env_var(shell, "SHLVL");
	if (current_shlvl)
		shlvl = ft_atoi(current_shlvl) + 1;
	else
		shlvl = 1;
	new_shlvl = ft_itoa(shlvl);
	if (new_shlvl)
	{
		set_env_var(shell, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
	if (shell->working_directory[0])
		set_env_var(shell, "PWD", shell->working_directory);
}

void	update_pwd_env(t_shell *shell, char *old_pwd, char *new_pwd)
{
	if (old_pwd && old_pwd[0])
		set_env_var(shell, "OLDPWD", old_pwd);
	if (new_pwd && new_pwd[0])
		set_env_var(shell, "PWD", new_pwd);
}

void	update_last_command(t_shell *shell, char *command)
{
	if (command && command[0])
		set_env_var(shell, "_", command);
}
