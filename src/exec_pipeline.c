/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:46:53 by trupham           #+#    #+#             */
/*   Updated: 2025/09/23 18:12:26 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t execute(t_cmd_table *cmd, t_pipe_line *pipeline, char **env)
{
	if (pipe(pipeline->pipe) < 0)
		return -1;
	pid_t child = fork();
	if (child == 0)
	{
		if (cmd->cmd_pos == FIRST)
		{
			close(pipeline->pipe[RD]);
			dup2(pipeline->pipe[WR], STDOUT_FILENO);
			close(pipeline->pipe[WR]);
			close(pipeline->tmp_fd);
		}
		if (cmd->cmd_pos == MID)
		{
			close(pipeline->pipe[RD]);
			dup2(pipeline->pipe[WR], STDOUT_FILENO);
			dup2(pipeline->tmp_fd, STDIN_FILENO);
			close(pipeline->pipe[WR]);
			close(pipeline->tmp_fd);
		}
		if (cmd->cmd_pos == LAST)
		{
			close(pipeline->pipe[WR]);
			close(pipeline->pipe[RD]);
			dup2(pipeline->tmp_fd, STDIN_FILENO);
			close(pipeline->tmp_fd);
		}
		execve(cmd->cmd_da->items[0], cmd->cmd_da->items, env);
	}
	pipeline->tmp_fd = dup(pipeline->pipe[RD]);
	close(pipeline->pipe[RD]);
	close(pipeline->pipe[WR]);
	return child;
}

/*looping through the command table and execute each command
 */
void pipeline(t_shell *shell)
{
    t_cmd_table *cmd = shell->cmd;
    pid_t child;
    int status;

    if (shell->state & HAS_PIPE) {
		while (cmd)
		{
			child = execute(cmd, shell->pipeline, shell->heap_env);
			cmd = cmd->next;
			waitpid(child, &shell->last_code, 0);
		}
    } else {
        child = fork();
        if (child == 0) {
            if (cmd->fd_in != STDIN_FILENO) {
                dup2(cmd->fd_in, STDIN_FILENO);
                close(cmd->fd_in);
            }
            if (cmd->fd_out != STDOUT_FILENO) {
                dup2(cmd->fd_out, STDOUT_FILENO);
                close(cmd->fd_out);
            }

            execve(cmd->cmd_da->items[0], cmd->cmd_da->items, shell->heap_env);
            exit(127);
        }
        else if (child > 0) {
            waitpid(child, &status, 0);
            shell->last_code = status;
            if (cmd->fd_in != STDIN_FILENO)
                close(cmd->fd_in);
            if (cmd->fd_out != STDOUT_FILENO)
                close(cmd->fd_out);
        }
        else {
            perror("fork");
            shell->code = 1;
        }
    }
}
