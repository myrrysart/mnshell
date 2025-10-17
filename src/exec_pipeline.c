/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:46:53 by trupham           #+#    #+#             */
/*   Updated: 2025/10/17 15:50:06 by trupham          ###   ########.fr       */
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
		setup_child_signals();
		exec_prep(cmd, pipeline);
		if (cmd->cmd_type == EXTERNAL)
			exec_extern(shell, cmd);
		else
		{
			builtin_select(shell, cmd);
			child_cleanup_and_exit(shell, cmd, shell->code);
		}
	}
	if (pipeline->tmp_fd != -1)
		close(pipeline->tmp_fd);
	pipeline->tmp_fd = dup(pipeline->pipe[RD]);
	clean_pipe(pipeline, cmd);
	return (child);
}

static void	clean_sig_pipe(t_shell *shell)
{
	if (shell->pipeline && shell->pipeline->tmp_fd != -1)
	{
		close(shell->pipeline->tmp_fd);
		shell->pipeline->tmp_fd = -1;
	}
}

void	exec_pipe_entry(t_shell *shell, t_cmd_table *cmd)
{
	pid_t	child;
	int		status;

	while (cmd)
	{
		child = exec_pipe(shell, cmd);
		cmd = cmd->next;
		waitpid(child, &status, 0);
		shell_update_code_from_status(shell, status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			clean_sig_pipe(shell);
			shell_abort_eval(shell, EXIT_SIGINT);
			break ;
		}
		if (!(shell->state & EVALUATING))
		{
			clean_sig_pipe(shell);
			break ;
		}
	}
}
