/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reporting.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:44:12 by trupham           #+#    #+#             */
/*   Updated: 2025/10/17 21:07:52 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sh_abort(t_shell *shell, t_shell_code code)
{
	if (code != OK)
		shell->code = code;
	shell->state &= ~(EVALUATING | HAS_PIPE | HAS_INPUT_REDIR
			| HAS_OUTPUT_REDIR | HAS_QUOTE | IN_SQUOTE
			| IN_DQUOTE | HEREDOC_EXPAND);
	g_received_signal = 0;
}

int	map_exec_errno_to_exit(int err)
{
	if (err == ENOENT)
		return (EXIT_CMD_NOT_FOUND);
	if (err == EACCES || err == EISDIR || err == ENOTDIR || err == ENOEXEC)
		return (EXIT_CMD_NOT_EXECUTABLE);
	return (EXIT_CMD_NOT_EXECUTABLE);
}

void	shell_update_code_from_status(t_shell *shell, int status)
{
	shell->code = status;
	if (WIFEXITED(status))
		shell->code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->code = 128 + WTERMSIG(status);
}

void	print_err(char *name, char *mes)
{
	ft_putstr_fd(name, 2);
	ft_putendl_fd(mes, 2);
}

void	errno_report(t_cmd_table *cmd)
{
	if (!cmd || !cmd->cmd_da || !cmd->cmd_da->items || !cmd->cmd_da->items[0])
		return ;
	if (errno == EACCES)
		print_err(cmd->cmd_da->items[0], ": Permission denied");
	else if (errno == ENOENT)
	{
		ft_putstr_fd(cmd->cmd_da->items[0], 2);
		if (ft_strchr(cmd->cmd_da->items[0], '/'))
			ft_putendl_fd(": No such file or directory", 2);
		else
			ft_putendl_fd(": command not found", 2);
	}
	else if (errno == ENOTDIR)
		print_err(cmd->cmd_da->items[0], ": Not a directory");
	else if (errno == EISDIR)
		print_err(cmd->cmd_da->items[0], ": Is a directory");
	else if (errno == ENOEXEC)
		print_err(cmd->cmd_da->items[0], ": Exec format error");
}
