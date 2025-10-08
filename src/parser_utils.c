/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:39:47 by trupham           #+#    #+#             */
/*   Updated: 2025/10/07 18:47:29 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strip_delimiter(t_shell *sh, t_token *token)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	sh->state |= HEREDOC_EXPAND;
	while (token->next->content[i] && j < 255)
	{
		if (token->next->content[i] == '\'' || token->next->content[i] == '"')
			sh->state &= ~HEREDOC_EXPAND;
		else
		{
			sh->heredoc_delim[sh->heredoc_index][j] = token->next->content[i];
			j++;
		}
		i++;
	}
}

void	parser_cmd_type(t_shell *shell, t_cmd_table *cmd, t_token *token)
{
	shell->state |= HAS_BUILTIN;
	if (ft_strncmp(token->content, "echo", 5) == 0)
		cmd->cmd_type = BUILTIN_ECHO;
	else if (ft_strncmp(token->content, "cd", 3) == 0)
		cmd->cmd_type = BUILTIN_CD;
	else if (ft_strncmp(token->content, "exit", 5) == 0)
		cmd->cmd_type = BUILTIN_EXIT;
	else if (ft_strncmp(token->content, "pwd", 4) == 0)
		cmd->cmd_type = BUILTIN_PWD;
	else if (ft_strncmp(token->content, "export", 6) == 0)
		cmd->cmd_type = BUILTIN_EXPORT;
	else if (ft_strncmp(token->content, "unset", 6) == 0)
		cmd->cmd_type = BUILTIN_UNSET;
	else if (ft_strncmp(token->content, "env", 6) == 0)
		cmd->cmd_type = BUILTIN_ENV;
	else
	{
		cmd->cmd_type = EXTERNAL;
		shell->state &= ~HAS_BUILTIN;
	}
}

static size_t	var_name_len(const char *s)
{
	size_t	i;

	if (*s == '?')
		return (1);
	if (!(*s == '_' || ft_isalpha(*s)))
		return (0);
	i = 1;
	while (s[i] && (s[i] == '_' || ft_isalnum(s[i])))
		i++;
	return (i);
}

static size_t	expand_one(t_shell *sh, t_buf *b, const char *s)
{
	size_t	n;
	char	save;
	char	*val;

	n = var_name_len(s + 1);
	if (n == 0)
	{
		buf_putc(b, '$');
		return (1);
	}
	save = s[1 + n];
	((char *)s)[1 + n] = '\0';
	val = expand_dollar_variable(sh, s + 1);
	if (val)
		buf_puts(b, val);
	((char *)s)[1 + n] = save;
	return (1 + n);
}

char	*parser_expand_dollar(t_shell *sh, char *str)
{
	t_buf	b;
	size_t	i;

	i = 0;
	if (!str || !ft_strchr(str, '$'))
		return (str);
	buf_init(&b, sh);
	while (str[i])
	{
		if (str[i] == '$')
			i += expand_one(sh, &b, str + i);
		else
			buf_putc(&b, str[i++]);
	}
	return (b.data);
}
