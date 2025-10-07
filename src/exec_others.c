/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_others.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 17:23:53 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/06 17:01:42 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_select(t_shell *shell, t_cmd_table *cmd)
{
	const builtin	builtin_table[BUILTIN_COUNT] = {
		builtin_cd,
		builtin_echo,
		builtin_exit,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
	};

	builtin_table[cmd->cmd_type](shell, cmd);
}

void	exec_child_no_pipe_prep(t_cmd_table *cmd)
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
}

void	exec_cleanup_parent(t_cmd_table *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}

void	exec_no_pipe(t_shell *shell)
{
	pid_t		child;
	t_cmd_table	*cmd;
	int			status;

	cmd = shell->cmd;
	if (cmd->cmd_type != EXTERNAL)
		return (builtin_select(shell, cmd));
	child = fork();
	if (child < 0)
	{
		perror("fork");
		shell->code = errno;
	}
	if (child == 0)
	{
		exec_child_no_pipe_prep(cmd);
		execve(cmd->cmd_da->items[0], cmd->cmd_da->items, shell->heap_env);
		exit(errno);
	}
	waitpid(child, &status, 0);
	shell->last_code = status;
	exec_cleanup_parent(cmd);
}

/*looping through the command table and execute each command
 */
void	exec_pipe(t_shell *shell)
{
	t_cmd_table	*cmd;
	pid_t		child;

	cmd = shell->cmd;
	while (cmd)
	{
		child = exec_pipeline(shell, cmd);
		cmd = cmd->next;
		waitpid(child, &shell->last_code, 0);
	}
}
