/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/16 19:27:24 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hd_setup_fail(t_shell *sh, struct sigaction *old_int, int *pfd)
{
	leave_heredoc(sh, old_int);
	close(pfd[RD]);
	pfd[RD] = -1;
	close(pfd[WR]);
	pfd[WR] = -1;
	return (-1);
}

static int	heredoc_cancelled(t_shell *sh)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	sh->code = EXIT_SIGINT;
	sh->state &= ~(EVALUATING | IN_HEREDOC | IN_SQUOTE | IN_DQUOTE | HAS_PIPE);
	return (HEREDOC_INTERRUPTED);
}

static void	heredoc_into_child(t_shell *sh, int *pfd)
{
	close(pfd[RD]);
	pfd[RD] = -1;
	heredoc_child(sh, pfd[WR], sh->heredoc_delim[sh->heredoc_index]);
}

static int	heredoc_generic_error(int *pfd, int return_nbr)
{
	if (return_nbr == -1)
	{
		close(pfd[RD]);
		pfd[RD] = -1;
		return (-1);
	}
	else if (return_nbr == -2)
	{
		ft_putendl_fd("minishell: maximum here-document count exceeded", 2);
		return (-1);
	}
	return (return_nbr);
}

int	read_heredoc(t_shell *sh)
{
	int					pfd[2] = {-1, -1};
	pid_t				pid;
	int					st;
	struct sigaction	old_int;
	int					w;

	if (sh->heredoc_count >= MAX_HEREDOCS)
		return (heredoc_generic_error(pfd, -2));
	if (pipe(pfd) < 0)
		return (-1);
	enter_heredoc(sh, &old_int);
	pid = fork();
	if (pid < 0)
		return (hd_setup_fail(sh, &old_int, pfd));
	if (pid == 0)
		heredoc_into_child(sh, pfd);
	close(pfd[WR]);
	pfd[WR] = -1;
	w = waitpid(pid, &st, 0);
	while (w == -1 && errno == EINTR)
		w = waitpid(pid, &st, 0);
	leave_heredoc(sh, &old_int);
	if (w == -1)
	{
		if (pfd[RD] != -1)
			close(pfd[RD]);
		pfd[RD] = -1;
		return (-1);
	}
	if ((WIFEXITED(st) && WEXITSTATUS(st) == 130) || (WIFSIGNALED(st)
			&& WTERMSIG(st) == SIGINT))
	{
		close(pfd[RD]);
		pfd[RD] = -1;
		return (heredoc_cancelled(sh));
	}
	if (!(WIFEXITED(st) && WEXITSTATUS(st) == 0))
	{
		close(pfd[RD]);
		pfd[RD] = -1;
		return (-1);
	}
	return (pfd[RD]);
}
