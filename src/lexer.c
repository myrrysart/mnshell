/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:45:01 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 12:06:46 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "minishell.h"

t_lexer	build_lexer(char *content)
{
	t_lexer	l;

	l.content = content;
	l.content_len = ft_strlen(content);
	l.cursor = 0;
	l.line = 0;
	return (l);
}

/*@brief: returning a token with a corresponding type, len and content
 * @param: pointer to the lexer structure. this funciton will modify the lexer
 * and moving the cursor forward
 */
t_token	get_next_token(t_shell *shell, t_lexer *l)
{
	t_token	token;

	ft_bzero(&token, sizeof(t_token));
	if (l->cursor >= l->content_len)
		return (token);
	trim_left(l);
	if (l->content[l->cursor] == '\'')
		return (lexer_handle_squote(l));
	if (l->content[l->cursor] == '"')
		return (lexer_handle_dquote(l));
	if (l->content[l->cursor] == '>' && l->content[l->cursor + 1] == '>')
		return (lexer_handle_append(l));
	if (l->content[l->cursor] == '<' && l->content[l->cursor + 1] == '<')
		return (lexer_handle_heredoc(l));
	if (l->content[l->cursor] == '$')
		return (lexer_handle_dollar(shell, l));
	if (!is_operator(l->content[l->cursor]) && !is_quote(l->content[l->cursor]))
		return (lexer_handle_word(l));
	return (lexer_handle_other_token(l));
}

/*@brief: allocate a token node in the memory arena
 * from a copy of token passed in
 *@return: pointer to the allocated token
 */
t_token	*build_token(t_arena *arena, t_token token)
{
	t_token	*new_token;
	size_t	i;

	i = 0;
	new_token = arena_alloc(arena, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = arena_alloc(arena, token.content_len + 1);
	if (!new_token->content)
		return (NULL);
	new_token->content_len = token.content_len;
	new_token->type = token.type;
	while (i < new_token->content_len && token.content[i])
	{
		new_token->content[i] = token.content[i];
		i++;
	}
	new_token->content[i] = '\0';
	return (new_token);
}

/*@brief: build a list of token from the lexer
 *@return: the head of the token list
 */
t_token	*build_token_list(t_shell *shell, t_lexer *l)
{
	t_token	*head;
	t_token	*new;
	t_token	*curr;

	head = build_token(sh_work_arena(shell), get_next_token(shell, l));
	curr = head;
	if (!head)
		return (NULL);
	while (l->cursor < l->content_len)
	{
		while (curr->next)
			curr = curr->next;
		new = build_token(sh_work_arena(shell), get_next_token(shell, l));
		if (!new)
			return (NULL);
		curr->next = new;
		new->prev = curr;
	}
	return (head);
}
