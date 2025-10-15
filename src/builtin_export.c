/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+      */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:41:58 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/09 13:12:55 by jyniemit         ###   ########.fr       */
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

static int	process_export_error(t_shell *shell, char *arg, int error_type)
{
	(void)shell;
	if (error_type == 1)
	{
		ft_putstr_fd("export: `", STDERR_FILENO); ft_putstr_fd(arg, STDERR_FILENO); ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (EXIT_GENERAL_ERROR);
	}
	else if (error_type == 2)
	{
		ft_putstr_fd("export: `", STDERR_FILENO); ft_putstr_fd(arg, STDERR_FILENO); ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (EXIT_GENERAL_ERROR);
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
			return (process_export_error(shell, arg, 1));
		set_env_var(shell, key, value);
	}
	else
	{
		if (!is_valid_identifier(arg))
			return (process_export_error(shell, arg, 2));
	}
	return (OK);
}

static int	ascii_cmp(const char *a, const char *b)
{
	const unsigned char	*sa;
	const unsigned char	*sb;

	sa = (const unsigned char *)a;
	sb = (const unsigned char *)b;
	while (*sa && *sb && *sa == *sb)
	{
		sa++;
		sb++;
	}
	return ((int)(*sa) - (int)(*sb));
}

static void	print_env_sorted_ascii(t_shell *shell)
{
	int		printed;
	int		idx;
	int		i;
	char	*min_str;
	char	*last;

	printed = 0;
	last = NULL;
	while (printed < shell->env_count)
	{
		idx = -1;
		min_str = NULL;
		i = -1;
		while (++i < shell->env_count)
		{
			if (!last)
			{
				if (idx == -1 || ascii_cmp(shell->heap_env[i], min_str) < 0)
				{
					idx = i;
					min_str = shell->heap_env[i];
				}
			}
			else if (ascii_cmp(shell->heap_env[i], last) > 0)
			{
				if (idx == -1 || ascii_cmp(shell->heap_env[i], min_str) < 0)
				{
					idx = i;
					min_str = shell->heap_env[i];
				}
			}
		}
		if (idx == -1)
			break ;
		i = -1;
		while (++i < shell->env_count)
			if (ascii_cmp(shell->heap_env[i], min_str) == 0)
			{
				ft_printf("%s\n", shell->heap_env[i]);
				printed++;
			}
		last = min_str;
	}
}

void	builtin_export(t_shell *shell, t_cmd_table *cmd)
{
	int	i;

	if (!shell)
	{
		shell->code = EXIT_BUILTIN_MISUSE;
		return ;
	}
	if (!cmd->cmd_da->items[1])
	{
		print_env_sorted_ascii(shell);
		shell->code = OK;
		return ;
	}
	i = 0;
	shell->code = OK;
	while (cmd->cmd_da->items[++i])
	{
		if (process_export_arg(shell, cmd->cmd_da->items[i]) != OK)
			shell->code = EXIT_GENERAL_ERROR;
	}
}
