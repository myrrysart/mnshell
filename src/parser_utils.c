/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:39:47 by trupham           #+#    #+#             */
/*   Updated: 2025/10/06 15:52:21 by trupham          ###   ########.fr       */
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
