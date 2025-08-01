/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:50:29 by jyniemit          #+#    #+#             */
/*   Updated: 2025/07/31 11:53:10 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "minishell.h"

char *print_token_type(t_token_type type)
{
	switch (type) {
		case END: return "token end";
		case SQUOTE: return "single quote";
		case DQUOTE: return "double quote";
		case CMD: return "command";
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
	const char *content = "echo hello grep < > | |  -s  $ $$ $ \' \"";
	t_lexer l = build_lexer(content);
	t_token t = {0};
	t = get_next_token(&l);
	while (t.type != END)
	{
		printf("%.*s (%s)\n", (int)t.text_len, t.text, print_token_type(t.type));
		t = get_next_token(&l);
	}
}

int	main2(int ac, char **av)
{
	t_shell	shell;

	init_signals(&shell);
	ft_bzero(&shell, sizeof(t_shell));
	init_shell(ac, av, &shell);
	run_shell(&shell);
	return (shell.code);
}
