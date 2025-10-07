/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:39:47 by trupham           #+#    #+#             */
/*   Updated: 2025/10/07 12:56:58 by jyniemit         ###   ########.fr       */
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

/* @brief: find length of the env key
 */
static size_t	ms_var_len(const char *s)
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

static void ms_increment(size_t *out_len, size_t *i)
{
	(*out_len)++;
	(*i)++;
}

static size_t	calculate_output_len(t_shell *sh, char *str, size_t out_len)
{
	size_t	i;
	size_t	env_key_len;
	char	save;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			env_key_len = ms_var_len(&str[i + 1]);
			if (!env_key_len)
				ms_increment(&out_len, &i);
			else
			{
				save = str[i + 1 + env_key_len];
				out_len += ft_strlen(expand_dollar_variable(sh, &str[i + 1]));
				str[i + 1 + env_key_len] = save;
				i += 1 + env_key_len;
			}
		}
		else
			ms_increment(&out_len, &i);
	}
	return out_len;
}

char	*parser_expand_dollar(t_shell *sh, char *str)
{
	size_t	i;
	size_t	j;
	size_t	n;
	char	save;
	char	*val;
	char	*out;

	if (!str || !ft_strchr(str, '$'))
		return (str);
	out = arena_alloc(sh_work_arena(sh), calculate_output_len(sh, str, 0) + 1);
	if (!out)
		return (str);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			n = ms_var_len(&str[i + 1]);
			if (!n)
				out[j++] = str[i++];
			else
			{
				save = str[i + 1 + n];
				str[i + 1 + n] = '\0';
				val = expand_dollar_variable(sh, &str[i + 1]);
				ft_memcpy(&out[j], val, ft_strlen(val));
				j += ft_strlen(val);
				str[i + 1 + n] = save;
				i += 1 + n;
			}
		}
		else
			out[j++] = str[i++];
	}
	out[j] = '\0';
	return (out);
}
