/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:40:26 by jyniemit          #+#    #+#             */
/*   Updated: 2025/08/19 12:18:34 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// PATH_MAX
# include <limits.h>
// STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
# include <unistd.h>
// readline
# include <stdio.h>
# include <stdlib.h>
# include <readline/history.h>
# include <readline/readline.h>
// sigaction & signals
# include <signal.h>
# include "libft.h"
# include "minishell.h"
# include "arena.h"

# define MAX_PROCESSES 30587
# define ARG_MAX 4096

extern volatile sig_atomic_t	g_received_signal;

typedef enum e_shell_code
{
	OK = 0,
	EXIT_GENERAL_ERROR = 1,
	EXIT_BUILTIN_MISUSE = 2,
	EXIT_CMD_NOT_EXECUTABLE = 126,
	EXIT_CMD_NOT_FOUND = 127,
	EXIT_SIGINT = 130,
	EXIT_SIGQUIT = 131,
	EXIT_SIGTERM = 143,
	EXIT_LINEFAIL = 65,
	EXIT_SHELLINITFAIL = 66,
	EXIT_PARSE_ERROR = 67,
	EXIT_REDIRECT_ERROR = 68,
	EXIT_CAN_RETRY = 123
}								t_shell_code;

typedef enum e_shell_state
{
	SHELL_DEFAULT = 0,
	EVALUATING = (1 << 0),
	WAITING_CHILD = (1 << 1),
	IN_HEREDOC = (1 << 2),
	NON_INTERACTIVE = (1 << 3),
	SHOULD_EXIT = (1 << 4),
	HAS_PIPE = (1 << 5),
	HAS_INPUT_REDIR = (1 << 6),
	HAS_OUTPUT_REDIR = (1 << 7),
	HAS_BUILTIN = (1 << 8),
	SUPPRESS_PROMPT = (1 << 9),
	IN_SQUOTE = (1 << 10),
	IN_DQUOTE = (1 << 11),
	ENV_MODIFIED = (1 << 10)
}								t_shell_state;

typedef struct s_shell
{
	t_shell_state				state;
	t_shell_code				code;
	t_shell_code				last_code;

	char						command_line[ARG_MAX];
	char						*args[ARG_MAX];
	int							argc;

	int							input_fd;
	int							output_fd;
	int							error_fd;

	int							pipe_read_fd;
	int							pipe_write_fd;

	pid_t						process_id[MAX_PROCESSES];
	int							process_exit_status[MAX_PROCESSES];

	char						**original_env;
	char						**heap_env;
	int							env_count;
	int							env_capacity;
	char						working_directory[PATH_MAX];

	int							token_index[ARG_MAX];
	int							token_types[ARG_MAX];
	int							token_count;

	struct sigaction			saved_sigint;
	struct sigaction			saved_sigquit;
	struct sigaction			saved_sigterm;
}								t_shell;

void							init_signals(t_shell *shell);
void							init_shell(int ac, char **av, t_shell *shell);
void							run_shell(t_shell *shell);
void							handle_signal(t_shell *shell, int sig);

#endif // MINISHELL_H
