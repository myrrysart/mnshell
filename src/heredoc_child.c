/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:49:16 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 18:15:09 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_delim_error_message(char *delim)
{
	ft_putstr_fd("minishell: warning: here-document delimited by end-of-file ",
		STDERR_FILENO);
	ft_putstr_fd("(wanted '", STDERR_FILENO);
	ft_putstr_fd(delim, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
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

int	hd_is_delim(char *line, char *delim)
{
	if (ft_strlen(line) == ft_strlen(delim) && !ft_strncmp(line, delim,
			ft_strlen(delim)))
		return (1);
	return (0);
}

void	hd_output_line(t_shell *sh, int write_fd, char *line, int expand)
{
	if (expand)
		hd_write_expansion(sh, line, write_fd);
	else
		write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
}

void	heredoc_child(t_shell *sh, int write_fd, char *delim)
{
	int	received_delim;

	received_delim = 0;
	sig_mode_set(SIGMODE_HEREDOC_CHILD);
	hd_read_loop(sh, write_fd, delim, &received_delim);
	if (g_received_signal == SIGINT)
	{
		close(write_fd);
		child_cleanup_and_exit(sh, NULL, 130);
	}
	if (!received_delim)
		heredoc_delim_error_message(delim);
	while (*delim)
	{
		*delim = '\0';
		delim++;
	}
	close(write_fd);
	child_cleanup_and_exit(sh, NULL, 0);
}
