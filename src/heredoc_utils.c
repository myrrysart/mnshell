/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:50:28 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/07 16:54:33 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_var_end(const char *s, int i)
{
	if (s[i] == '?')
		return (i + 1);
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (i);
}

static void	expand_dollar_token(t_shell *sh, t_buf *b, const char *line, int *i)
{
	int		start;
	int		end;
	char	*name;
	char	*exp;
	int		diff;

	(*i)++;
	start = *i;
	end = parse_var_end(line, *i);
	diff = end - start;
	if (diff <= 0)
	{
		buf_putc(b, '$');
		return ;
	}
	name = arena_alloc(sh_work_arena(sh), diff + 1);
	if (!name)
		return ;
	ft_strlcpy(name, &line[start], diff + 1);
	exp = expand_dollar_variable(sh, name);
	if (exp)
		buf_puts(b, exp);
	*i = end;
}

char	*expand_heredoc_line(t_shell *sh, const char *line)
{
	int		i;
	t_buf	b;

	buf_init(&b, sh);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			expand_dollar_token(sh, &b, line, &i);
		else
		{
			buf_putc(&b, line[i]);
			i++;
		}
	}
	return (b.data);
}
