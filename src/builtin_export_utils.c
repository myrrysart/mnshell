/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 18:17:35 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/17 18:19:22 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	find_smaller(t_shell *shell, int *idx, char **min_str, char *last)
{
	int	i;

	i = -1;
	while (++i < shell->env_count)
	{
		if (!last)
		{
			if (*idx == -1 || ascii_cmp(shell->heap_env[i], *min_str) < 0)
			{
				*idx = i;
				*min_str = shell->heap_env[i];
			}
		}
		else if (ascii_cmp(shell->heap_env[i], last) > 0)
		{
			if (*idx == -1 || ascii_cmp(shell->heap_env[i], *min_str) < 0)
			{
				*idx = i;
				*min_str = shell->heap_env[i];
			}
		}
	}
}

void	print_env_sorted_ascii(t_shell *shell)
{
	int		printed;
	int		idx;
	char	*min_str;
	char	*last;

	printed = 0;
	last = NULL;
	while (printed < shell->env_count)
	{
		idx = -1;
		min_str = NULL;
		find_smaller(shell, &idx, &min_str, last);
		if (idx == -1)
			break ;
		idx = -1;
		while (++idx < shell->env_count)
		{
			if (ascii_cmp(shell->heap_env[idx], min_str) == 0)
			{
				ft_printf("%s\n", shell->heap_env[idx]);
				printed++;
			}
		}
		last = min_str;
	}
}
