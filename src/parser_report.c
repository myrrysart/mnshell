/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_report.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:29:34 by trupham           #+#    #+#             */
/*   Updated: 2025/10/17 15:35:08 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* redirect & heredoc helpers */
bool	ensure_next_is_word(t_token *tok)
{
	if (!tok || !tok->next)
		return (false);
	return (tok->next->type == WORD || tok->next->type == SQUOTE
		|| tok->next->type == DQUOTE);
}

void	print_redir_error(char *path)
{
	ft_putstr_fd(path, 2);
	if (errno == EACCES)
		ft_putendl_fd(": Permission denied", 2);
	else if (errno == ENOENT)
		ft_putendl_fd(": No such file or directory", 2);
	else if (errno == EISDIR)
		ft_putendl_fd(": Is a directory", 2);
	else if (errno == ENOTDIR)
		ft_putendl_fd(": Not a directory", 2);
	else
		ft_putendl_fd(": Redirection error", 2);
}
