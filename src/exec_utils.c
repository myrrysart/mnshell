/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:09:42 by trupham           #+#    #+#             */
/*   Updated: 2025/10/17 14:54:08 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_getenv(char **env, char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (ft_substr(env[i], len + 1, ft_strlen(env[i]) - len - 1));
		i++;
	}
	return (NULL);
}

static void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static char	*get_full_cmd(char **arr, char *cmd)
{
	int			i;
	char		*tmp_cmd;
	char		*full_cmd;
	struct stat	st;

	i = 0;
	while (arr[i])
	{
		tmp_cmd = ft_strjoin(arr[i], "/");
		if (!tmp_cmd)
			return (NULL);
		full_cmd = ft_strjoin(tmp_cmd, cmd);
		if (!full_cmd)
			return (free(tmp_cmd), NULL);
		free(tmp_cmd);
		if (stat(full_cmd, &st) == 0 && S_ISREG(st.st_mode) && access(full_cmd,
				X_OK) == 0)
			return (full_cmd);
		free(full_cmd);
		i++;
	}
	return (NULL);
}
/*
 * @brief: takes a command string and search for binary file in PATH
 * @params: name of the command eg: ls, cat, etc.
 * @params: array of strings containing environment variables
 * @return: the whole path to the command: /usr/bin/cat
 */

static char	*exec_get_binary_path(char *cmd, char **env)
{
	char	*path;
	char	**arr;
	char	*full_cmd;

	path = ft_getenv(env, "PATH");
	if (!path)
		return (NULL);
	arr = ft_split(path, ':');
	if (!arr)
		return (free(path), NULL);
	free(path);
	full_cmd = get_full_cmd(arr, cmd);
	if (!full_cmd)
		return (free_split(arr), free(full_cmd), NULL);
	return (free_split(arr), full_cmd);
}

char	*get_path(t_shell *shell, char *cmd)
{
	char	*bin_cmd;
	char	*arena_cmd;

	if (ft_strlen(cmd) <= 0)
		return (NULL);
	if (ft_strncmp(cmd, ".", 2) == 0)
		return (cmd);
	if (ft_strchr(cmd, '/'))
		return (cmd);
	bin_cmd = exec_get_binary_path(cmd, shell->heap_env);
	if (!bin_cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Command not found.", 2);
		shell_abort_eval(shell, EXIT_CMD_NOT_FOUND);
		return (cmd);
	}
	arena_cmd = arena_alloc(sh_work_arena(shell), ft_strlen(bin_cmd) + 1);
	if (!arena_cmd)
		return (free(bin_cmd), NULL);
	ft_memcpy(arena_cmd, bin_cmd, ft_strlen(bin_cmd) + 1);
	free(bin_cmd);
	return (arena_cmd);
}
