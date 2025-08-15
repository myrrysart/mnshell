/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:12:10 by trupham           #+#    #+#             */
/*   Updated: 2025/08/08 15:12:48 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"
#include "arena.h"

char *print_token_type2(t_token_type type)
{
	switch (type) {
		case END: return "token end";
		case SQUOTE: return "single quote";
		case DQUOTE: return "double quote";
		case WORD: return "word";
		case HEREDOC: return "heredoc";
		case REDIRECT_IN: return "redirect in";
		case REDIRECT_OUT: return "redirect out";
		case PIPE: return "pipe";
		case APPEND: return "append";
		case DOLLAR: return "dollar sign";
		default: return "invalid";
	}
}

int main()
{
	// char *l1 = "hello world | >out | wc -l >> \'";
	char *l1 = "| >> $|    $     >>   < \' echo hello world \" ./usr/bin/ls";
	t_lexer l = build_lexer(l1);
	t_token *t = build_token_list(&l);
	while (t)
	{
		printf("current token (%s): %s\n", print_token_type2(t->type), t->text);
		t = t->next;
	}
	arena_free(get_static_arena());
}
