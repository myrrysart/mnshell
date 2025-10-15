/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:17:04 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/09 11:17:10 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_apply_redirs(t_cmd_table *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
		cmd->fd_out = STDOUT_FILENO;
	}
}

void	exec_builtin_with_redirs(t_shell *shell, t_cmd_table *cmd)
{
	int	backup_in;
	int	backup_out;

	backup_in = dup(STDIN_FILENO);
	backup_out = dup(STDOUT_FILENO);
	if (backup_in < 0 || backup_out < 0)
		return ;
	exec_apply_redirs(cmd);
	builtin_select(shell, cmd);
	dup2(backup_in, STDIN_FILENO);
	dup2(backup_out, STDOUT_FILENO);
	close(backup_in);
	close(backup_out);
}
