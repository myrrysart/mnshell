/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:49:16 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/09 11:32:58 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_delim_error_message(char *delim)
{
	ft_putstr_fd("minishell: warning: here-document delimited \
by end-of-file (wanted \'", STDERR_FILENO);
	ft_putstr_fd(delim, STDERR_FILENO);
	ft_putstr_fd("\')\n", STDERR_FILENO);
}

static void	hd_write_expansion(t_shell *sh, char *line, int write_fd)
{
	char	*proc;

	proc = expand_heredoc_line(sh, line);
	if (proc)
		write(write_fd, proc, ft_strlen(proc));
	else
		write(write_fd, line, ft_strlen(line));
}

static void	hd_proc_readl(t_shell *sh, int write_fd, char *delim, int *flag)
{
	int		expand;
	char	*line;

	expand = (sh->state & HEREDOC_EXPAND) != 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strlen(line) == ft_strlen(delim) && !ft_strncmp(line, delim,
				ft_strlen(delim)))
		{
			free(line);
			*flag = 1;
			break ;
		}
		if (expand)
			hd_write_expansion(sh, line, write_fd);
		else
			write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

static void	heredoc_sigint(int sig)
{
	(void)sig;
	exit(130);
}

void	heredoc_child(t_shell *sh, int write_fd, char *delim)
{
	struct sigaction	sa;
	int					received_delim;

	received_delim = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_DFL);
	hd_proc_readl(sh, write_fd, delim, &received_delim);
	close(write_fd);
	if (!received_delim)
		heredoc_delim_error_message(delim);
	exit(0);
}
