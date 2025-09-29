/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:58 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/02 15:42:23 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	i = 0;
	while (str[++i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
	}
	return (1);
}

static int	process_export_arg(t_shell *shell, char *arg)
{
	char	*equals_pos;
	char	*key;
	char	*value;
	int		key_len;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		key_len = equals_pos - arg;
		key = arena_alloc(shell->arena, key_len + 1);
		if (!key)
			return (EXIT_GENERAL_ERROR);
		ft_strlcpy(key, arg, key_len + 1);
		value = equals_pos + 1;
		if (!is_valid_identifier(key))
		{
			ft_printf("export: `%s': not a valid identifier\n", arg);
			return (EXIT_GENERAL_ERROR);
		}
		set_env_var(shell, key, value);
	}
	else
	{
		if (!is_valid_identifier(arg))
		{
			ft_printf("export: `%s': not a valid identifier\n", arg);
			return (EXIT_GENERAL_ERROR);
		}
	}
	return (OK);
}

void	builtin_export(t_shell *shell, t_cmd_table *cmd)
{
	int	i;

	if (!shell)
	{
		shell->code = EXIT_BUILTIN_MISUSE;
		return;
	}
	if (!cmd->cmd_da->items[1])
	{
		builtin_env(shell, cmd);
		return;
	}
	i = 0;
	shell->code = OK;
	while (cmd->cmd_da->items[++i])
	{
		if (process_export_arg(shell, cmd->cmd_da->items[i]) != OK)
			shell->code = EXIT_GENERAL_ERROR;
	}
	
}
