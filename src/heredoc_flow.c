/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_flow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 18:20:00 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 18:07:44 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_wait_and_restore(t_shell *sh, pid_t pid, int *st,
				struct sigaction *old_int)
{
	int	w;

	w = waitpid(pid, st, 0);
	while (w == -1 && errno == EINTR)
		w = waitpid(pid, st, 0);
	leave_heredoc(sh, old_int);
	return (w);
}

int	heredoc_status_result(t_shell *sh, int *pfd, int st, int w)
{
	if (w == -1)
		return (heredoc_generic_error(pfd, -1));
	if ((WIFEXITED(st) && WEXITSTATUS(st) == 130)
		|| (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT))
		return (heredoc_cancelled(sh, pfd));
	if (!(WIFEXITED(st) && WEXITSTATUS(st) == 0))
		return (heredoc_generic_error(pfd, -1));
	return (pfd[RD]);
}

void	hd_read_loop(t_shell *sh, int write_fd, char *delim, int *flag)
{
	char	*line;
	int		expand;

	expand = (sh->state & HEREDOC_EXPAND) != 0;
	while (1)
	{
		line = readline("> ");
		if (g_received_signal == SIGINT)
		{
			if (line)
				free(line);
			break ;
		}
		if (!line)
			break ;
		if (ft_strlen(line) == ft_strlen(delim) && !ft_strncmp(line, delim,
				ft_strlen(delim)))
		{
			free(line);
			*flag = 1;
			break ;
		}
		hd_output_line(sh, write_fd, line, expand);
		free(line);
	}
}
