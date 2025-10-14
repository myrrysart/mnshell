/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:46:53 by trupham           #+#    #+#             */
/*   Updated: 2025/10/13 20:14:13 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	exec_pipeline(t_shell *shell, t_cmd_table *cmd)
{
	t_pipe_line	*pipeline;
	pid_t		child;
	struct stat	st;

	pipeline = shell->pipeline;
	if (pipe(pipeline->pipe) < 0)
		return (-1);
	child = fork();
	if (child == 0)
	{
		setup_child_signals();
		exec_prep(cmd, pipeline);
		if (cmd->cmd_type == EXTERNAL)
		{
			if (cmd->cmd_da && cmd->cmd_da->count == 1
				&& ft_strncmp(cmd->cmd_da->items[0], ".", 2) == 0)
			{
				ft_putendl_fd(".: filename argument required", 2);
				child_cleanup_and_exit(shell, cmd, EXIT_BUILTIN_MISUSE);
			}
			if (cmd->cmd_da && cmd->cmd_da->items[0])
			{
				if (stat(cmd->cmd_da->items[0], &st) == 0
					&& S_ISDIR(st.st_mode))
				{
					ft_putstr_fd(cmd->cmd_da->items[0], 2);
					ft_putendl_fd(": Is a directory", 2);
					child_cleanup_and_exit(shell, cmd, EXIT_CMD_NOT_EXECUTABLE);
				}
			}
			if (!cmd->cmd_da || !cmd->cmd_da->items[0]) {
				ft_putendl_fd("Command not found.", 2);
				child_cleanup_and_exit(shell, cmd, EXIT_CMD_NOT_FOUND);
			}
			execve(cmd->cmd_da->items[0], cmd->cmd_da->items, shell->heap_env);
			if (errno == EACCES)
			{
				ft_putstr_fd(cmd->cmd_da->items[0], 2);
				ft_putendl_fd(": Permission denied", 2);
			}
			else if (errno == ENOENT)
			{
				ft_putstr_fd(cmd->cmd_da->items[0], 2);
				if (ft_strchr(cmd->cmd_da->items[0], '/'))
					ft_putendl_fd(": No such file or directory", 2);
				else
					ft_putendl_fd(": Command not found.", 2);
			}
			else if (errno == ENOTDIR)
			{
				ft_putstr_fd(cmd->cmd_da->items[0], 2);
				ft_putendl_fd(": Not a directory", 2);
			}
			else if (errno == EISDIR)
			{
				ft_putstr_fd(cmd->cmd_da->items[0], 2);
				ft_putendl_fd(": Is a directory", 2);
			}
			else if (errno == ENOEXEC)
			{
				ft_putstr_fd(cmd->cmd_da->items[0], 2);
				ft_putendl_fd(": Exec format error", 2);
			}
			child_cleanup_and_exit(shell, cmd, map_exec_errno_to_exit(errno));
		}
		else
		{
			builtin_select(shell, cmd);
			child_cleanup_and_exit(shell, cmd, shell->code);
		}
	}
	if (pipeline->tmp_fd != -1) close(pipeline->tmp_fd);
	pipeline->tmp_fd = dup(pipeline->pipe[RD]);
	close_pipe(pipeline);
	return (child);
}
