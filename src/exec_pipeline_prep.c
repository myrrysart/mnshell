/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_prep.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:04:58 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 17:05:54 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(t_pipe_line *pipe)
{
	close(pipe->pipe[RD]);
	close(pipe->pipe[WR]);
}

static void	exec_first_prep(t_cmd_table *cmd, t_pipe_line *pipeline)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	else
		dup2(pipeline->pipe[WR], STDOUT_FILENO);
	close_pipe(pipeline);
}

static void	exec_mid_prep(t_cmd_table *cmd, t_pipe_line *pipeline)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	else
		dup2(pipeline->pipe[WR], STDOUT_FILENO);
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	else
	{
		dup2(pipeline->tmp_fd, STDIN_FILENO);
	}
	close_pipe(pipeline);
	if (pipeline->tmp_fd != -1)
		close(pipeline->tmp_fd);
}

static void	exec_last_prep(t_cmd_table *cmd, t_pipe_line *pipeline)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	close_pipe(pipeline);
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	else
	{
		dup2(pipeline->tmp_fd, STDIN_FILENO);
	}
	if (pipeline->tmp_fd != -1)
		close(pipeline->tmp_fd);
}

void	exec_prep(t_cmd_table *cmd, t_pipe_line *pipeline)
{
	if (cmd->cmd_pos == FIRST)
		exec_first_prep(cmd, pipeline);
	if (cmd->cmd_pos == MID)
		exec_mid_prep(cmd, pipeline);
	if (cmd->cmd_pos == LAST)
		exec_last_prep(cmd, pipeline);
}
