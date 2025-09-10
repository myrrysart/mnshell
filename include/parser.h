/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:54:35 by trupham           #+#    #+#             */
/*   Updated: 2025/08/25 14:09:52 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include <stdbool.h>
#include <stddef.h>
typedef enum {
	BUILTINT,
	EXTERNAL
} e_cmd;

typedef struct s_shell t_shell;

typedef struct s_da
{
	char **items;
	size_t count;
	size_t cap;
} t_da;

typedef struct s_cmd
{
	t_da *cmd_da;
	e_cmd cmd_type;
	int fd_in;
	int fd_out;
	struct s_cmd *next;
	struct s_cmd *prev;
} t_cmd_table;

#ifndef DA_CAP
#define DA_CAP 128
#endif // DA_CAP

void *ft_realloc(t_arena *arena, char **src, size_t src_size, size_t new_size);
t_da *da_cmd_init(t_arena *arena, size_t cap);
void parser_da_append(char **da, char *str);
void da_append(t_arena *arena, t_da *da, char *item);
t_cmd_table *parser_cmd_build_one(t_arena *arena, t_token *token);
t_cmd_table *parser_cmd_build_many(t_arena *arena, t_token *token);

bool parser_is_syntax_correct(t_token *token);
#endif // PARSER_H
