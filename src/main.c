/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:50:29 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/07 11:38:35 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include "arena.h"

int	main1(int ac, char **av)
{
	t_shell	shell;

	init_signals(&shell);
	ft_bzero(&shell, sizeof(t_shell));
	init_shell(ac, av, &shell);
	run_shell(&shell);
	return (shell.code);
}
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
