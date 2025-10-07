/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/07 16:17:44 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hd_setup_fail(t_shell *sh, struct sigaction *old_int, int *pfd)
{
	leave_heredoc(sh, old_int);
	close(pfd[RD]);
	close(pfd[WR]);
	return (-1);
}

static int	heredoc_cancelled(t_shell *sh, int *pfd)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	sh->code = EXIT_SIGINT;
	sh->command_line[0] = '\0';
	sh->state &= ~(EVALUATING | IN_HEREDOC | IN_SQUOTE | IN_DQUOTE | HAS_PIPE);
	close(pfd[RD]);
	return (-2);
}

static void	heredoc_into_child(t_shell *sh, int *pfd)
{
	close(pfd[RD]);
	heredoc_child(sh, pfd[WR], sh->heredoc_delim[sh->heredoc_index]);
}

static int	heredoc_generic_error(int *pfd)
{
	close(pfd[RD]);
	return (-1);
}

int	read_heredoc(t_shell *sh)
{
	int					pfd[2];
	pid_t				pid;
	int					st;
	struct sigaction	old_int;

	if (sh->heredoc_index > 8)
		return (-1);
	if (pipe(pfd) < 0)
		return (-1);
	enter_heredoc(sh, &old_int);
	pid = fork();
	if (pid < 0)
		return (hd_setup_fail(sh, &old_int, pfd));
	if (pid == 0)
		heredoc_into_child(sh, pfd);
	close(pfd[WR]);
	while (waitpid(pid, &st, 0) == -1 && errno == EINTR)
		;
	leave_heredoc(sh, &old_int);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 130)
		return (heredoc_cancelled(sh, pfd));
	if (!(WIFEXITED(st) && WEXITSTATUS(st) == 0))
		return (heredoc_generic_error(pfd));
	return (pfd[RD]);
}
