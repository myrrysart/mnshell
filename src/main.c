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

#include "minishell.h"

int	main(int ac, char **av)
{
	t_shell	shell;

	shell = (t_shell){};
	init_signals(&shell);
	init_shell(ac, av, &shell);
	run_shell(&shell);
	return (shell.code);
}
