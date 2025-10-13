/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/08 14:53:25 by jyniemit         ###   ########.fr       */
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
	sh->state &= ~(EVALUATING | IN_HEREDOC | IN_SQUOTE | IN_DQUOTE | HAS_PIPE);
	close(pfd[RD]);
	return (-2);
}

static void	heredoc_into_child(t_shell *sh, int *pfd)
{
	close(pfd[RD]);
	heredoc_child(sh, pfd[WR], sh->heredoc_delim[sh->heredoc_index]);
}

static int	heredoc_generic_error(int *pfd, int return_nbr)
{
	if (return_nbr == -1)
	{
		close(pfd[RD]);
		return (-1);
	}
	else if (return_nbr == -2)
	{
		ft_putendl_fd("minishell: maximum here-document count exceeded", 2);
		return (-1);
	}
	return (return_nbr);
}

static void	hd_parent_write_line(t_shell *sh, int write_fd, char *line, int expand)
{
	if (expand)
	{
		char *proc = expand_heredoc_line(sh, line);
		if (proc)
			write(write_fd, proc, ft_strlen(proc));
		else
			write(write_fd, line, ft_strlen(line));
	}
	else
		write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
}

int	read_heredoc(t_shell *sh)
{
	int					pfd[2];
	pid_t				pid;
	int					st;
	struct sigaction	old_int;

	if (sh->heredoc_count > 8)
		return (heredoc_generic_error(pfd, -2));
	if (pipe(pfd) < 0)
		return (-1);
	/* In non-interactive mode, consume from parent's GNL buffer */
	if (!isatty(STDIN_FILENO))
	{
		int expand = (sh->state & HEREDOC_EXPAND) != 0;
		char *line;
		while (1)
		{
			line = get_next_line(STDIN_FILENO);
			if (!line)
			{
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted '", STDERR_FILENO);
				ft_putstr_fd(sh->heredoc_delim[sh->heredoc_index], STDERR_FILENO);
				ft_putendl_fd("')", STDERR_FILENO);
				break;
			}
			size_t len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				line[len - 1] = '\0';
			if (ft_strlen(line) == ft_strlen(sh->heredoc_delim[sh->heredoc_index])
				&& !ft_strncmp(line, sh->heredoc_delim[sh->heredoc_index], ft_strlen(sh->heredoc_delim[sh->heredoc_index])))
			{
				free(line);
				break;
			}
			hd_parent_write_line(sh, pfd[WR], line, expand);
			free(line);
		}
		/* clear delimiter buffer */
		char *d = sh->heredoc_delim[sh->heredoc_index];
		while (*d)
			*(d++) = '\0';
		close(pfd[WR]);
		return (pfd[RD]);
	}
	/* Interactive: fork and read via readline */
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
		return (heredoc_generic_error(pfd, -1));
	return (pfd[RD]);
}
