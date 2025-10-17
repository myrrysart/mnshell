/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:24:35 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 13:17:08 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	not_std(int fd)
{
	if (fd > STDERR_FILENO)
		return (1);
	return (0);
}

void	child_cleanup_and_exit(t_shell *sh, t_cmd_table *cmd, int status)
{
	if (cmd)
	{
		if (not_std(cmd->fd_in))
		{
			close(cmd->fd_in);
			cmd->fd_in = STDIN_FILENO;
		}
		if (not_std(cmd->fd_out))
		{
			close(cmd->fd_out);
			cmd->fd_out = STDOUT_FILENO;
		}
	}
	if (sh && sh->pipeline)
	{
		if (not_std(sh->pipeline->pipe[RD]))
			close(sh->pipeline->pipe[RD]);
		if (not_std(sh->pipeline->pipe[WR]))
			close(sh->pipeline->pipe[WR]);
		if (not_std(sh->pipeline->tmp_fd))
			close(sh->pipeline->tmp_fd);
	}
	if (sh)
	{
		if (sh->frame_arena)
			arena_free(sh->frame_arena);
		if (sh->arena)
			arena_free(sh->arena);
	}
	exit(status);
}

void close_all_cmd_fds(t_cmd_table *cmd)
{
    while (cmd)
    {
		if (not_std(cmd->fd_in))
		{
			close(cmd->fd_in);
			cmd->fd_in = STDIN_FILENO;
		}
		if (not_std(cmd->fd_out))
		{
			close(cmd->fd_out);
			cmd->fd_out = STDOUT_FILENO;
		}
        cmd = cmd->next;
    }
}
