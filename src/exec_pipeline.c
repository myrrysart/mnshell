/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:46:53 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 17:05:50 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	exec_pipeline(t_shell *shell, t_cmd_table *cmd)
{
	t_pipe_line	*pipeline;
	pid_t		child;

	pipeline = shell->pipeline;
	if (pipe(pipeline->pipe) < 0)
		return (-1);
	child = fork();
	if (child == 0)
	{
		exec_prep(cmd, pipeline);
		if (cmd->cmd_type == EXTERNAL)
			execve(cmd->cmd_da->items[0], cmd->cmd_da->items, shell->heap_env);
		else
		{
			builtin_select(shell, cmd);
			exit(0);
		}
	}
	pipeline->tmp_fd = dup(pipeline->pipe[RD]);
	close_pipe(pipeline);
	return (child);
}
