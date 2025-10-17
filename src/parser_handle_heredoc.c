/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handle_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 19:12:46 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 19:26:13 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	reading_result(t_shell *sh)
{
	int	newfd;

	newfd = read_heredoc(sh);
	if (newfd == HEREDOC_INTERRUPTED)
	{
		sh->heredoc_index--;
		sh_abort(sh, EXIT_SIGINT);
		return (-1);
	}
	if (newfd < 0)
	{
		sh->heredoc_index--;
		sh_abort(sh, EXIT_HEREDOC_ERROR);
		return (-1);
	}
	return (newfd);
}

bool	handle_heredoc(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!*tok || !(*tok)->next || !(*tok)->next->content)
		return (sh_abort(sh, EXIT_PARSE_ERROR), false);
	sh->heredoc_index++;
	strip_delimiter(sh, *tok);
	if (cmd->fd_in > STDERR_FILENO)
	{
		close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO;
	}
	cmd->fd_in = reading_result(sh);
	if (cmd->fd_in == -1)
		return (false);
	sh->state |= HAS_INPUT_REDIR;
	cmd->heredoc_index = sh->heredoc_index;
	*tok = (*tok)->next->next;
	return (true);
}
