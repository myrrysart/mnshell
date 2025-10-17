/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utilities.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:30:47 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 21:54:40 by jyniemit         ###   ########.fr       */
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

void	builtin_env(t_shell *shell, t_cmd_table *cmd)
{
	int	i;

	(void)cmd;
	if (!shell)
	{
		shell->code = EXIT_BUILTIN_MISUSE;
		return ;
	}
	i = -1;
	while (++i < shell->env_count)
		ft_printf("%s\n", shell->heap_env[i]);
	shell->code = OK;
}

char	*expand_dollar_variable(t_shell *shell, const char *var_name)
{
	char	*value;
	char	*tmp;
	char	*arena_copy;

	if (!var_name || !*var_name)
		return (arena_strdup(sh_work_arena(shell), ""));
	if (var_name[0] == '?' && var_name[1] == '\0')
	{
		if (var_name[0] == '?' && var_name[1] == '\0'
				&& g_received_signal == SIGINT && shell->state & EVALUATING)
			tmp = ft_itoa(130);
		else
			tmp = ft_itoa(shell->code);
		if (!tmp)
			return (arena_strdup(sh_work_arena(shell), "0"));
		arena_copy = arena_strdup(sh_work_arena(shell), tmp);
		free(tmp);
		return (arena_copy);
	}
	value = get_env_var(shell, (char *)var_name);
	if (value)
		return (arena_strdup(sh_work_arena(shell), value));
	return (arena_strdup(sh_work_arena(shell), ""));
}
