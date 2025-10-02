/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:27:57 by trupham           #+#    #+#             */
/*   Updated: 2025/09/23 18:31:04 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static bool parser_is_quote_count_correct(t_token *token)
{
	int i;
	size_t count_squote;
	size_t count_dquote;

	count_squote = 0;
	count_dquote = 0;
	while (token)
	{
		i = 0;
		while (token->content[i])
		{
			if (token->content[i] == '\"')
				count_dquote++;
			if (token->content[i] == '\'')
				count_squote++;
			i++;
		}
		token = token->next;
	}
	if (count_dquote % 2 != 0 || count_squote % 2 != 0)
		return false;
	return true;
}

/* checking for PIPE, REDIRECTION and APPEND syntax
 */
bool	parser_is_syntax_correct(t_token *token)
{
	t_token	*last;

	if (!token || !parser_is_quote_count_correct(token))
		return false;
	last = token;
	while (last->next)
		last = last->next;
	if (last->type == PIPE || last->type == REDIRECT_OUT
		|| last->type == REDIRECT_IN || last->type == APPEND
		|| last->type == HEREDOC)
		return (false);
	while (token && token->next)
	{
		if (token->type == PIPE && token->next->type != WORD)
			return false;
		if ((token->type == REDIRECT_OUT || token->type == REDIRECT_IN
		|| token->type == APPEND || token->type == HEREDOC)
			&& token->next->type != WORD)
			return false;
		if (token->type == PIPE && !token->prev)
			return false;
		token = token->next;
	}
	return (true);
}

/* @brief: main function to construct a single command struct
 * @params: shell pointer and the token pointer containing command information
 * @return: a command table pointer
 */
t_cmd_table *parser_cmd_build_one(t_shell *shell, t_token *token)
{
	t_cmd_table *new_cmd;
	int			i;

	new_cmd = arena_alloc(shell->arena, sizeof(*new_cmd));
	i = -1;
	if (!new_cmd)
		return NULL;
	new_cmd->fd_in = STDIN_FILENO;
	new_cmd->fd_out = STDOUT_FILENO;
	new_cmd->cmd_da = da_cmd_init(shell->arena, DA_CAP);
	if (!new_cmd->cmd_da)
		return NULL;
	new_cmd->cmd_pos = MID;
	parser_cmd_type(shell, new_cmd, token);
	while (token && token->type != PIPE)
	{
		if (token->type == REDIRECT_OUT)
		{
			new_cmd->fd_out = open(token->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (new_cmd->fd_out == -1)
			{
				shell->code = EXIT_REDIRECT_ERROR;
				return (NULL);
			}
			token = token->next->next;
		}
		else if (token->type == REDIRECT_IN)
		{
			new_cmd->fd_in = open(token->next->content, O_RDONLY);
			if (new_cmd->fd_in == -1)
			{
				shell->code = EXIT_REDIRECT_ERROR;
				return (NULL);
			}
			token = token->next->next;
		}
		else if (token->type == APPEND)
		{
			new_cmd->fd_out = open(token->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (new_cmd->fd_out == -1)
			{
				shell->code = EXIT_REDIRECT_ERROR;
				return (NULL);
			}
			token = token->next->next;
		}
		else if (token->type == HEREDOC)
		{
			shell->heredoc_index++;
			while (token->next->content[++i] && i < 255)
				shell->heredoc_delim[shell->heredoc_index][i] = token->next->content[i];
			shell->heredoc_delim[shell->heredoc_index][i] = '\0';
			new_cmd->fd_in = read_heredoc(shell);
			if (new_cmd->fd_in == -1)
			{
				shell->code = EXIT_HEREDOC_ERROR;
				return (NULL);
			}
			new_cmd->heredoc_index = shell->heredoc_index;
			token = token->next->next;
		}
		else if (ft_strchr(token->content, '\'') || ft_strchr(token->content, '"'))
		{
			da_append(shell->arena, new_cmd->cmd_da, ft_strtrim(token->content, "'\""));
			token = token->next;
		}
		else
		{
			char *cmd = exec_copy_bin_path(shell, token->content);
			if (cmd)
				da_append(shell->arena, new_cmd->cmd_da, cmd);
			token = token->next;
		}
	}
	return new_cmd;
}

static bool parser_cmd_build_curr(t_shell *shell, t_cmd_table **curr, t_token *token)
{
	t_cmd_table *new_cmd;

	new_cmd = parser_cmd_build_one(shell, token);
	if (!new_cmd)
		return false;
	(*curr)->next = new_cmd;
	new_cmd->prev = *curr;
	return true;
}

/* @brief: main loop that handles subsequents commands.
 * first it skips the first token since it should be a PIPE.
 * it then traverse the command table passed in as head to find the last command (curr).
 * token_end denotes the end of the current command. token->next is the start of the current command.
 * cmd_build_curr will build the current command given the start of the command.
 * if token end is a PIPE, move the token pointer to the PIPE to prepare for the next pipe command.
 * if token end is NULL (end of list), we terminate
 */
static bool parser_cmd_build_main(t_shell *shell, t_cmd_table *head, t_token *token)
{
	t_token *token_end;
	t_cmd_table *curr;

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
				return false;
			break;
		}
		else if (token_end && token_end->type == PIPE)
		{
			if (!parser_cmd_build_curr(shell, &curr, token->next))
				return false;
			token = token_end;
		}
	}
	return true;
}

void parser_cmd_type(t_shell *shell, t_cmd_table *cmd, t_token *token)
{
	shell->state |= HAS_BUILTIN;
	if (ft_strncmp(token->content, "echo", 5) == 0)
		cmd->cmd_type = BUILTIN_ECHO;
	else if (ft_strncmp(token->content, "cd", 3) == 0)
		cmd->cmd_type = BUILTIN_CD;
	else if (ft_strncmp(token->content, "exit", 5) == 0)
		cmd->cmd_type = BUILTIN_EXIT;
	else if (ft_strncmp(token->content, "pwd", 4) == 0)
		cmd->cmd_type = BUILTIN_PWD;
	else if (ft_strncmp(token->content, "export", 6) == 0)
		cmd->cmd_type = BUILTIN_EXPORT;
	else if (ft_strncmp(token->content, "unset", 6) == 0)
		cmd->cmd_type = BUILTIN_UNSET;
	else if (ft_strncmp(token->content, "env", 6) == 0)
		cmd->cmd_type = BUILTIN_ENV;
	else
	{
		cmd->cmd_type = EXTERNAL;
		shell->state &= ~HAS_BUILTIN;
	}
}

/* @brief: entry to the parser
 * first it builds the head of the command table then traverse the token list
 * to PIPE which is the delimiter that separates different commands.
 * it then performs the main loop inside the cmd_build_main function
 * @params: shell pointer and token pointer
 * @return: head of the linked list for command table
 */
t_cmd_table *parser_cmd_build_many(t_shell *shell, t_token *token)
{
	t_cmd_table *cmd_table_head;
	t_cmd_table *curr;

	if (!token)
		return NULL;
	cmd_table_head = parser_cmd_build_one(shell, token);
	if (!cmd_table_head)
		return NULL;
	parser_cmd_type(shell, cmd_table_head, token);
	cmd_table_head->cmd_pos = FIRST;
	while (token && token->type != PIPE)
		token = token->next;
	if (!parser_cmd_build_main(shell, cmd_table_head, token))
		return NULL;
	curr = cmd_table_head;
	while (curr->next)
		curr = curr->next;
	curr->cmd_pos = LAST;
	return cmd_table_head;
}

/* realloc the buf pointer but in the arena
 */
void *ft_realloc(t_arena *arena, char **src, size_t src_size, size_t new_size)
{
	char **new_ptr;
	size_t i;

	if (new_size == 0)
		return NULL;
	new_ptr = arena_alloc(arena, sizeof(*new_ptr) * new_size);
	if (!new_ptr)
		return NULL;
	i = 0;
	while (i < src_size)
	{
		new_ptr[i] = arena_strdup(arena, src[i]);
		i++;
	}
	return new_ptr;
}

/* initialize the dynamic array for the command arguments
 */
t_da *da_cmd_init(t_arena *arena, size_t cap)
{
	t_da *da;

	da = arena_alloc(arena, sizeof(*da));
	if (!da)
		return NULL;
	da->cap = cap;
	da->count = 0;
	da->items = arena_alloc(arena, sizeof(*(da->items)) * cap);
	if (!da->items)
		return NULL;
	return da;
}

void da_append(t_arena *arena, t_da *da, char *item)
{
	if (da->count + 1 >= da->cap)
	{
		while (da->count + 1 >= da->cap)
			da->cap *= 2;
		da->items = ft_realloc(arena, da->items, da->count + 1, da->cap);
	}
	da->items[da->count++] = arena_strdup(arena, item);
}
