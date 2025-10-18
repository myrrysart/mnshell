/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:46:53 by trupham           #+#    #+#             */
/*   Updated: 2025/10/17 20:42:18 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_extern(t_shell *shell, t_cmd_table *cmd)
{
	struct stat	st;

	if (cmd->cmd_da && cmd->cmd_da->count == 1
		&& ft_strncmp(cmd->cmd_da->items[0], ".", 2) == 0)
	{
		ft_putendl_fd(".: filename argument required", 2);
		child_cleanup_and_exit(shell, cmd, EXIT_BUILTIN_MISUSE);
	}
	if (cmd->cmd_da && cmd->cmd_da->items[0])
	{
		if (stat(cmd->cmd_da->items[0], &st) == 0 && S_ISDIR(st.st_mode))
		{
			print_err(cmd->cmd_da->items[0], ": Is a directory");
			child_cleanup_and_exit(shell, cmd, EXIT_CMD_NOT_EXECUTABLE);
		}
	}
	if (!cmd->cmd_da || !cmd->cmd_da->items[0])
		child_cleanup_and_exit(shell, cmd, OK);
	execve(cmd->cmd_da->items[0], cmd->cmd_da->items, shell->heap_env);
	errno_report(cmd);
	child_cleanup_and_exit(shell, cmd, map_exec_errno_to_exit(errno));
}

static void	clean_pipe(t_pipe_line *pipeline, t_cmd_table *cmd)
{
	if (pipeline->tmp_fd != -1)
		close(pipeline->tmp_fd);
	pipeline->tmp_fd = dup(pipeline->pipe[RD]);
	if (cmd->fd_in > STDERR_FILENO)
	{
		close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out > STDERR_FILENO)
	{
		close(cmd->fd_out);
		cmd->fd_out = STDOUT_FILENO;
	}
	close_pipe(pipeline);
}

static pid_t	exec_pipe(t_shell *shell, t_cmd_table *cmd)
{
	t_pipe_line	*pipeline;
	pid_t		child;

	pipeline = shell->pipeline;
	if (pipe(pipeline->pipe) < 0)
		return (-1);
	child = fork();
	if (child < 0)
		fork_error(shell, cmd);
	if (child == 0)
	{
		sig_mode_set(SIGMODE_CHILD);
		exec_prep(cmd, pipeline);
		if (cmd->cmd_type == EXTERNAL)
			exec_extern(shell, cmd);
		else
		{
			builtin_select(shell, cmd);
			child_cleanup_and_exit(shell, cmd, shell->code);
		}
	}
	clean_pipe(pipeline, cmd);
	return (child);
}

void	exec_pipe_entry(t_shell *shell, t_cmd_table *cmd)
{
	pid_t	child;
	pid_t	last_child;
	int		count;

	last_child = -1;
	count = 0;
	while (cmd)
	{
		child = exec_pipe(shell, cmd);
		if (child > 0)
		{
			last_child = child;
			count++;
		}
		cmd = cmd->next;
	}
	exec_pipeline_wait_and_finalize(shell, last_child, count);
}
