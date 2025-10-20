/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handle_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 19:12:46 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/20 11:12:09 by jyniemit         ###   ########.fr       */
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

static bool	check_if_not_part_of_delimiter(t_token *walker)
{
	if (!walker->next || walker->next->wp
		|| !((walker->next->type == WORD) || (walker->next->type == SQUOTE)
			|| (walker->next->type == DQUOTE)))
		return (true);
	return (false);
}

static t_token	*strip_delimiter(t_shell *sh, t_token *token)
{
	int		i;
	int		j;
	t_token	*walker;

	j = 0;
	sh->state |= HEREDOC_EXPAND;
	walker = token->next;
	while (walker && (walker->type == WORD || walker->type == SQUOTE
			|| walker->type == DQUOTE))
	{
		i = 0;
		while (walker->content[i] && j < 255)
		{
			if (walker->content[i] == '\'' || walker->content[i] == '"')
				sh->state &= ~HEREDOC_EXPAND;
			else
				sh->heredoc_delim[sh->heredoc_index][j++] = walker->content[i];
			i++;
		}
		if (check_if_not_part_of_delimiter(walker))
			return (walker);
		walker = walker->next;
	}
	return (walker);
}

bool	handle_heredoc(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!*tok || !(*tok)->next || !(*tok)->next->content)
		return (sh_abort(sh, EXIT_PARSE_ERROR), false);
	sh->heredoc_index++;
	*tok = strip_delimiter(sh, *tok);
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
	if (*tok)
		*tok = (*tok)->next;
	else
		*tok = NULL;
	return (true);
}
