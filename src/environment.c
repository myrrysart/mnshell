/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:46:03 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/07 17:32:22 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_env_capacity(t_shell *shell)
{
	char	**new_env;
	int		i;

	shell->env_capacity *= 2;
	new_env = arena_alloc(shell->arena, sizeof(char *) * (shell->env_capacity
				+ 1));
	i = -1;
	while (++i < shell->env_count)
		new_env[i] = shell->heap_env[i];
	new_env[shell->env_count] = NULL;
	shell->heap_env = new_env;
}

static void	update_shell_env_data(t_shell *shell, char *key, size_t key_len,
		char *value)
{
	char	*new_var;

	new_var = arena_alloc(shell->arena, key_len + ft_strlen(value) + 2);
	ft_strlcpy(new_var, key, key_len + 1);
	ft_strlcat(new_var, "=", key_len + 2);
	ft_strlcat(new_var, value, key_len + ft_strlen(value) + 2);
	shell->heap_env[shell->env_count] = new_var;
	shell->env_count++;
	shell->heap_env[shell->env_count] = NULL;
	shell->state |= ENV_MODIFIED;
}

void	set_env_var(t_shell *shell, char *key, char *value)
{
	char	*new_var;
	int		i;
	size_t	key_len;

	if (!shell || !key)
		return ;
	key_len = ft_strlen(key);
	i = -1;
	while (++i < shell->env_count)
	{
		if (ft_strncmp(shell->heap_env[i], key, key_len) == 0
			&& shell->heap_env[i][key_len] == '=')
		{
			new_var = arena_alloc(shell->arena, key_len + ft_strlen(value) + 2);
			ft_strlcpy(new_var, key, key_len + 1);
			ft_strlcat(new_var, "=", key_len + 2);
			ft_strlcat(new_var, value, key_len + ft_strlen(value) + 2);
			shell->heap_env[i] = new_var;
			shell->state |= ENV_MODIFIED;
			return ;
		}
	}
	if (shell->env_count >= shell->env_capacity - 1)
		expand_env_capacity(shell);
	update_shell_env_data(shell, key, key_len, value);
}

void	unset_env_var(t_shell *shell, char *key)
{
	int		i;
	int		j;
	size_t	key_len;

	if (!shell || !key)
		return ;
	key_len = ft_strlen(key);
	i = -1;
	while (++i < shell->env_count)
	{
		if (ft_strncmp(shell->heap_env[i], key, key_len) == 0
			&& shell->heap_env[i][key_len] == '=')
		{
			j = i;
			while (j < shell->env_count - 1)
			{
				shell->heap_env[j] = shell->heap_env[j + 1];
				j++;
			}
			shell->env_count--;
			shell->heap_env[shell->env_count] = NULL;
			shell->state |= ENV_MODIFIED;
			return ;
		}
	}
}
