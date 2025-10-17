/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:27:57 by trupham           #+#    #+#             */
/*   Updated: 2025/10/17 17:26:41 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table	*parser_cmd_build_one(t_shell *shell, t_token *tok)
{
	t_cmd_table	*cmd;
	int		first;

	cmd = arena_alloc(sh_work_arena(shell), sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	cmd->cmd_da = da_cmd_init(sh_work_arena(shell), DA_CAP);
	if (!cmd->cmd_da)
		return (NULL);
	cmd->cmd_pos = MID;
	parser_cmd_type(shell, cmd, tok);
	first = 1;
	while (tok && tok->type != PIPE)
	{
		if (!handle_token(shell, cmd, &tok, &first))
		{
			clean_up_fds(cmd);
			return (NULL);
		}
	}
	if (cmd->cmd_da && cmd->cmd_da->count == 0)
		clean_up_fds(cmd);
	return (cmd);
	if (cmd->cmd_da && cmd->cmd_da->count == 0)
	{
		if (cmd->fd_in != STDIN_FILENO) { close(cmd->fd_in); cmd->fd_in = STDIN_FILENO; }
		if (cmd->fd_out != STDOUT_FILENO) { close(cmd->fd_out); cmd->fd_out = STDOUT_FILENO; }
	}
	return (cmd);
}

static bool	parser_cmd_build_curr(t_shell *shell, t_cmd_table **curr,
		t_token *token)
{
	t_cmd_table	*new_cmd;

	new_cmd = parser_cmd_build_one(shell, token);
	if (!new_cmd)
		return (false);
	(*curr)->next = new_cmd;
	new_cmd->prev = *curr;
	return (true);
}

/* @brief: main loop that handles subsequents commands.
 * first it skips the first token since it should be a PIPE.
 * it then traverse the command table passed in as
 * head to find the last command (curr).
 * token_end denotes the end of the current command.
 * token->next is the start of the current command.
 * cmd_build_curr will build the current command given the start of the command.
 * if token end is a PIPE,
 * move the token pointer to the PIPE to prepare for the next pipe command.
 * if token end is NULL (end of list), we terminate
 */
static bool	parser_cmd_build_main(t_shell *shell, t_cmd_table *head,
		t_token *token)
{
	t_token		*token_end;
	t_cmd_table	*curr;

	curr = head;
	while (token && token->next)
	{
		token_end = token->next;
		while (curr->next)
			curr = curr->next;
		while (token_end && token_end->type != PIPE)
			token_end = token_end->next;
		if (!token_end)
		{
			if (!parser_cmd_build_curr(shell, &curr, token->next))
				return (false);
			break ;
		}
		else if (token_end && token_end->type == PIPE)
		{
			if (!parser_cmd_build_curr(shell, &curr, token->next))
				return (false);
			token = token_end;
		}
	}
	return (true);
}

static void	*null_return_and_close_fds(t_cmd_table *cmd)
{
	close_all_cmd_fds(cmd);
	return (NULL);
}

/* @brief: entry to the parser
 * first it builds the head of the command table then traverse the token list
 * to PIPE which is the delimiter that separates different commands.
 * it then performs the main loop inside the cmd_build_main function
 * @params: shell pointer and token pointer
 * @return: head of the linked list for command table
 */
t_cmd_table	*parser_cmd_build_many(t_shell *shell, t_token *token)
{
	t_cmd_table	*cmd_table_head;
	t_cmd_table	*curr;

	if (!token)
		return (NULL);
	cmd_table_head = parser_cmd_build_one(shell, token);
	if (!cmd_table_head)
		return (NULL);
	parser_cmd_type(shell, cmd_table_head, token);
	cmd_table_head->cmd_pos = FIRST;
	while (token && token->type != PIPE)
		token = token->next;
	if (!parser_cmd_build_main(shell, cmd_table_head, token))
		return (null_return_and_close_fds(cmd_table_head));
	curr = cmd_table_head;
	while (curr->next)
		curr = curr->next;
	curr->cmd_pos = LAST;
	if (!(shell->state & EVALUATING))
		return (null_return_and_close_fds(cmd_table_head));
	return (cmd_table_head);
}
