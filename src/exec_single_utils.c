/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 04:20:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/18 04:20:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cleanup_parent(t_cmd_table *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}

void	exec_child_prep(t_shell *shell, t_cmd_table *cmd)
{
	struct stat	st;

	sig_mode_set(SIGMODE_CHILD);
	exec_apply_redirs(cmd);
	if (cmd->cmd_da && cmd->cmd_da->count == 1
		&& ft_strncmp(cmd->cmd_da->items[0], ".", 2) == 0)
	{
		ft_putendl_fd(".: filename argument required", 2);
		child_cleanup_and_exit(shell, cmd, EXIT_BUILTIN_MISUSE);
	}
	if (cmd->cmd_da && cmd->cmd_da->items[0]
		&& stat(cmd->cmd_da->items[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_err(cmd->cmd_da->items[0], ": Is a directory");
		child_cleanup_and_exit(shell, cmd, EXIT_CMD_NOT_EXECUTABLE);
	}
	if (!cmd->cmd_da || !cmd->cmd_da->items[0])
		child_cleanup_and_exit(shell, cmd, OK);
}
