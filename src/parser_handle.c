/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:41:48 by trupham           #+#    #+#             */
/*   Updated: 2025/10/14 18:20:00 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* redirect & heredoc helpers */
static bool	ensure_next_is_word(t_token *tok)
{
	if (!tok || !tok->next)
		return (false);
	return (tok->next->type == WORD || tok->next->type == SQUOTE
		|| tok->next->type == DQUOTE);
}

static void	print_redir_error(char *path)
{
	ft_putstr_fd(path, 2);
	if (errno == EACCES)
		ft_putendl_fd(": Permission denied", 2);
	else if (errno == ENOENT)
		ft_putendl_fd(": No such file or directory", 2);
	else if (errno == EISDIR)
		ft_putendl_fd(": Is a directory", 2);
	else if (errno == ENOTDIR)
		ft_putendl_fd(": Not a directory", 2);
	else
		ft_putendl_fd(": Redirection error", 2);
}

static bool	handle_redirect_out(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!ensure_next_is_word(*tok))
		return (shell_abort_eval(sh, EXIT_PARSE_ERROR), false);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open((*tok)->next->content, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (cmd->fd_out == -1)
	{
		print_redir_error((*tok)->next->content);
		return (shell_abort_eval(sh, EXIT_REDIRECT_ERROR), false);
	}
	sh->state |= HAS_OUTPUT_REDIR;
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_redirect_in(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!ensure_next_is_word(*tok))
		return (shell_abort_eval(sh, EXIT_PARSE_ERROR), false);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = open((*tok)->next->content, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		print_redir_error((*tok)->next->content);
		return (shell_abort_eval(sh, EXIT_REDIRECT_ERROR), false);
	}
	sh->state |= HAS_INPUT_REDIR;
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_append(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	if (!ensure_next_is_word(*tok))
		return (shell_abort_eval(sh, EXIT_PARSE_ERROR), false);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	cmd->fd_out = open((*tok)->next->content, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (cmd->fd_out == -1)
	{
		print_redir_error((*tok)->next->content);
		return (shell_abort_eval(sh, EXIT_REDIRECT_ERROR), false);
	}
	sh->state |= HAS_OUTPUT_REDIR;
	*tok = (*tok)->next->next;
	return (true);
}

static bool	handle_heredoc(t_shell *sh, t_cmd_table *cmd, t_token **tok)
{
	sh->heredoc_index++;
	strip_delimiter(sh, *tok);
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = read_heredoc(sh);
	if (cmd->fd_in == HEREDOC_INTERRUPTED)
		return (sh->heredoc_index--, shell_abort_eval(sh, EXIT_SIGINT), false);
	if (cmd->fd_in < 0)
		return (sh->heredoc_index--, shell_abort_eval(sh, EXIT_HEREDOC_ERROR),
			false);
	cmd->heredoc_index = sh->heredoc_index;
	sh->state |= HAS_INPUT_REDIR;
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
