/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_buf_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:04:24 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/07 15:19:06 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	buf_init(t_buf *b, t_shell *sh)
{
	b->cap = 128;
	b->len = 0;
	b->sh = sh;
	b->data = arena_alloc(sh_work_arena(sh), b->cap);
	if (b->data)
		b->data[0] = '\0';
}

void	buf_grow(t_buf *b, size_t need)
{
	char	*new_buf;
	size_t	new_cap;

	if (b->len + need + 1 <= b->cap)
		return ;
	new_cap = b->cap;
	while (b->len + need + 1 > new_cap)
		new_cap *= 2;
	new_buf = arena_alloc(sh_work_arena(b->sh), new_cap);
	if (!new_buf)
		return ;
	ft_strlcpy(new_buf, b->data, new_cap);
	b->data = new_buf;
	b->cap = new_cap;
}

void	buf_putc(t_buf *b, char c)
{
	buf_grow(b, 1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
}

void	buf_puts(t_buf *b, const char *s)
{
	size_t	sl;

	if (!s)
		return ;
	sl = ft_strlen(s);
	if (sl == 0)
		return ;
	buf_grow(b, sl);
	ft_strlcpy(&b->data[b->len], s, b->cap - b->len);
	b->len += sl;
}
