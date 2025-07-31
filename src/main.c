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

int main()
{
	const char *content = "<< < > >> test \'\" ";
	t_lexer l = build_lexer(content);
	t_token t = {0};
	t = get_next_token(&l);
	while (t.type != END)
	{
		ft_printf("%s %d\n", t.text, t.type);
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
