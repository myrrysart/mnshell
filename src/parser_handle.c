/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:41:48 by trupham           #+#    #+#             */
/*   Updated: 2025/10/07 19:01:35 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* redirect & heredoc helpers */
static bool	handle_redirect_out(t_shell *shell, t_cmd_table *cmd, t_token **tok)
{
	cmd->fd_out = open((*tok)->next->content, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (cmd->fd_out == -1)
	{
		shell->code = EXIT_REDIRECT_ERROR;
		return (false);
	}
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_redirect_in(t_shell *shell, t_cmd_table *cmd, t_token **tok)
{
	cmd->fd_in = open((*tok)->next->content, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		shell->code = EXIT_REDIRECT_ERROR;
		return (false);
	}
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_append(t_shell *shell, t_cmd_table *cmd, t_token **tok)
{
	cmd->fd_out = open((*tok)->next->content, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (cmd->fd_out == -1)
	{
		shell->code = EXIT_REDIRECT_ERROR;
		return (false);
	}
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_heredoc(t_shell *shell, t_cmd_table *cmd, t_token **tok)
{
	shell->heredoc_index++;
	strip_delimiter(shell, *tok);
	cmd->fd_in = read_heredoc(shell);
	if (cmd->fd_in == HEREDOC_INTERRUPTED)
	{
		shell->heredoc_index--;
		return (false);
	}
	if (cmd->fd_in < 0)
	{
		shell->heredoc_index--;
		shell->code = EXIT_HEREDOC_ERROR;
		return (false);
	}
	cmd->heredoc_index = shell->heredoc_index;
	*tok = (*tok)->next->next;
	return (true);
}

bool	handle_token(t_shell *shell, t_cmd_table *cmd, t_token **tok,
		int *first)
{
	if ((*tok)->type == REDIRECT_OUT)
		return (handle_redirect_out(shell, cmd, tok));
	else if ((*tok)->type == REDIRECT_IN)
		return (handle_redirect_in(shell, cmd, tok));
	else if ((*tok)->type == APPEND)
		return (handle_append(shell, cmd, tok));
	else if ((*tok)->type == HEREDOC)
		return (handle_heredoc(shell, cmd, tok));
	return (handle_word(shell, cmd, tok, first));
}
