/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:09:42 by trupham           #+#    #+#             */
/*   Updated: 2025/10/13 15:32:18 by jyniemit         ###   ########.fr       */
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
		if (ft_strncmp(env[i], name, len) == 0)
			return (ft_substr(env[i], len + 1, ft_strlen(env[i]) - len - 1));
		i++;
	}
	return (NULL);
}

static void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static char	*get_full_cmd(char **arr, char *cmd)
{
	int		i;
	char	*tmp_cmd;
	char	*full_cmd;

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
		if (access(full_cmd, F_OK | X_OK) == 0)
			return (full_cmd);
		else
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
		return (NULL);
	ft_memcpy(arena_cmd, bin_cmd, ft_strlen(bin_cmd));
	free(bin_cmd);
	return (arena_cmd);
}

void	shell_abort_eval(t_shell *shell, t_shell_code code)
{
	if (code != OK)
		shell->code = code;
	shell->state &= ~(EVALUATING | HAS_PIPE | HAS_INPUT_REDIR | HAS_OUTPUT_REDIR | HAS_QUOTE | IN_SQUOTE | IN_DQUOTE | HEREDOC_EXPAND);
	g_received_signal = 0;
}
int	map_exec_errno_to_exit(int err)
{
	if (err == ENOENT)
		return (EXIT_CMD_NOT_FOUND);
	if (err == EACCES || err == EISDIR || err == ENOTDIR)
		return (EXIT_CMD_NOT_EXECUTABLE);
	return (EXIT_CMD_NOT_EXECUTABLE);
}

void	shell_update_code_from_status(t_shell *shell, int status)
{
	shell->last_code = status;
	if (WIFEXITED(status))
		shell->code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->code = 128 + WTERMSIG(status);
}

