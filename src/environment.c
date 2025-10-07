/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:46:03 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/07 12:53:32 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var(t_shell *shell, char *key)
{
	int		i;
	size_t	len;

	if (!shell || !key)
		return (NULL);
	len = ft_strlen(key);
	i = -1;
	while (++i < shell->env_count)
	{
		if (ft_strncmp(shell->heap_env[i], key, len) == 0
			&& shell->heap_env[i][len] == '=')
			return (&shell->heap_env[i][len + 1]);
	}
	return (NULL);
}

char *expand_dollar_variable(t_shell *shell, const char *var_name)
{
    char *value;
    if (!var_name || !*var_name)
        return arena_strdup(sh_work_arena(shell), "");
    if (var_name[0] == '?' && var_name[1] == '\0')
    {
        char *tmp = ft_itoa(shell->code);
        if (!tmp)
            return arena_strdup(sh_work_arena(shell), "0");
        char *arena_copy = arena_strdup(sh_work_arena(shell), tmp);
        free(tmp);
        return arena_copy;
    }
    value = get_env_var(shell, (char *)var_name);
    if (value)
        return arena_strdup(sh_work_arena(shell), value);
    return arena_strdup(sh_work_arena(shell), "");
}

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
	new_var = arena_alloc(shell->arena, key_len + ft_strlen(value) + 2);
	ft_strlcpy(new_var, key, key_len + 1);
	ft_strlcat(new_var, "=", key_len + 2);
	ft_strlcat(new_var, value, key_len + ft_strlen(value) + 2);
	shell->heap_env[shell->env_count] = new_var;
	shell->env_count++;
	shell->heap_env[shell->env_count] = NULL;
	shell->state |= ENV_MODIFIED;
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

void	builtin_env(t_shell *shell, t_cmd_table *cmd)
{
	int	i;

	(void)cmd;
	if (!shell)
	{
		shell->code = EXIT_BUILTIN_MISUSE;
		return;
	}
	i = -1;
	while (++i < shell->env_count)
		ft_printf("%s\n", shell->heap_env[i]);
	shell->code = OK;
}
