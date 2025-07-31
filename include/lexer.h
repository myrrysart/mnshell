/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:45:22 by trupham           #+#    #+#             */
/*   Updated: 2025/07/31 11:53:10 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum {
	END = 0,
	SQUOTE,
	DQUOTE,
	CMD,
	HEREDOC,
	REDIRECT_IN,
	REDIRECT_OUT,
	PIPE,
	APPEND,
} t_token_type ;

typedef struct s_token
{
	t_token_type type;
	size_t text_len;
	const char *text;
} t_token;

typedef struct s_lexer
{
	const char *content;
	size_t content_len;
	size_t cursor;
	size_t line;
} t_lexer;

t_lexer build_lexer(const char *content);
t_token *get_next_token(t_lexer *l);
#endif // !LEXER_H
