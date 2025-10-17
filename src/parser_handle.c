/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:41:48 by trupham           #+#    #+#             */
/*   Updated: 2025/10/17 19:27:08 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_redirect_out(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!ensure_next_is_word(*tok))
	{
		sh_abort(sh, EXIT_PARSE_ERROR);
		return (false);
	}
	if (cmd->fd_out > STDERR_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open((*tok)->next->content, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (cmd->fd_out < 0)
	{
		print_redir_error((*tok)->next->content);
		sh_abort(sh, EXIT_REDIRECT_ERROR);
		return (false);
	}
	sh->state |= HAS_OUTPUT_REDIR;
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_redirect_in(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!ensure_next_is_word(*tok))
		return (sh_abort(sh, EXIT_PARSE_ERROR), false);
	if (cmd->fd_in > STDERR_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = open((*tok)->next->content, O_RDONLY);
	if (cmd->fd_in < 0)
	{
		print_redir_error((*tok)->next->content);
		sh_abort(sh, EXIT_REDIRECT_ERROR);
		return (false);
	}
	sh->state |= HAS_INPUT_REDIR;
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_append(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!ensure_next_is_word(*tok))
		return (sh_abort(sh, EXIT_PARSE_ERROR), false);
	if (cmd->fd_out > STDERR_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open((*tok)->next->content, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (cmd->fd_out < 0)
	{
		print_redir_error((*tok)->next->content);
		sh_abort(sh, EXIT_REDIRECT_ERROR);
		return (false);
	}
	sh->state |= HAS_OUTPUT_REDIR;
	*tok = (*tok)->next->next;
	return (true);
}

bool	handle_token(t_shell *sh, t_cmd_table *cmd, t_token **tok, int *first)
{
	if ((*tok)->type == REDIRECT_OUT)
		return (handle_redirect_out(sh, cmd, tok));
	else if ((*tok)->type == REDIRECT_IN)
		return (handle_redirect_in(sh, cmd, tok));
	else if ((*tok)->type == APPEND)
		return (handle_append(sh, cmd, tok));
	else if ((*tok)->type == HEREDOC)
		return (handle_heredoc(sh, cmd, tok));
	return (handle_word(sh, cmd, tok, first));
}
