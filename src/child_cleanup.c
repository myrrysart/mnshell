#include "minishell.h"

static void close_if_not_std(int fd)
{
	if (fd != -1 && fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		close(fd);
}

void	child_cleanup_and_exit(t_shell *sh, t_cmd_table *cmd, int status)
{
	if (cmd)
	{
		close_if_not_std(cmd->fd_in);
		close_if_not_std(cmd->fd_out);
	}
	if (sh && sh->pipeline)
	{
		close_if_not_std(sh->pipeline->pipe[RD]);
		close_if_not_std(sh->pipeline->pipe[WR]);
		close_if_not_std(sh->pipeline->tmp_fd);
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
