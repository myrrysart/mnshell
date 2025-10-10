/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:40:26 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/10 11:57:32 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PROMPT "\033[32mminishell$ \033[0m"

// PATH_MAX
# include <errno.h>
# include <limits.h>
// STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
# include <unistd.h>
// readline
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
// sigaction & signals
# include <signal.h>
// uint typedefinitions
# include <stdint.h>
// our own libraries
# include "libft.h"
// parser libraries
# include <fcntl.h>
# include <stdbool.h>
# include <stddef.h>
# include <sys/wait.h>
# define MAX_PROCESSES 30587
# define ARG_MAX 4096
# define WR 1
# define RD 0
# define MAX_HEREDOCS 16
# define HEREDOC_INTERRUPTED -2
# ifndef DA_CAP
#  define DA_CAP 128
# endif // DA_CAP
# ifndef STR_CAP
#  define STR_CAP 16
# endif // DA_CAP
extern volatile sig_atomic_t	g_received_signal;

// arena prototype
# ifndef ARENA_CAP
#  define ARENA_CAP 16384
# endif // !ARENA_CAP

typedef struct s_arena
{
	uint64_t					capacity;
	uint64_t					cursor;
	unsigned char				*data;
	struct s_arena				*next;
}								t_arena;

t_arena							*arena_init(uint64_t capacity);
void							*arena_alloc(t_arena *arena, uint64_t size);
void							*s_malloc(uint64_t size);
char							*arena_strdup(t_arena *arena, const char *s);
void							arena_free(t_arena *arena);
t_arena							*get_static_arena(void);

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
	EXIT_CAN_RETRY = 123,
	EXIT_HEREDOC_ERROR = 112,
}								t_shell_code;

typedef enum e_shell_state
{
	SHELL_DEFAULT = 0,
	EVALUATING = (1u << 0),
	WAITING_CHILD = (1u << 1),
	SHOULD_EXIT = (1u << 2),
	NON_INTERACTIVE = (1u << 3),
	IN_HEREDOC = (1u << 4),
	IN_SQUOTE = (1u << 5),
	IN_DQUOTE = (1u << 6),
	HAS_PIPE = (1u << 7),
	HAS_INPUT_REDIR = (1u << 8),
	HAS_OUTPUT_REDIR = (1u << 9),
	HAS_BUILTIN = (1u << 10),
	SUPPRESS_PROMPT = (1u << 11),
	ENV_MODIFIED = (1u << 12),
	HEREDOC_EXPAND = (1u << 13),
	HAS_QUOTE = (1u << 14)
}								t_shell_state;

typedef struct s_pipe_line
{
	int							pipe[2];
	int							tmp_fd;
}								t_pipe_line;

typedef struct s_cmd			t_cmd_table;
typedef struct s_token			t_token;

typedef struct s_shell
{
	t_shell_state				state;
	t_shell_code				code;
	int							last_code;

	char						command_line[ARG_MAX];
	char						*args[ARG_MAX];
	int							argc;

	t_token						*token;
	t_cmd_table					*cmd;
	t_pipe_line					*pipeline;
	pid_t						process_id[MAX_PROCESSES];
	int							process_exit_status[MAX_PROCESSES];

	char						**original_env;
	char						**heap_env;
	int							env_count;
	int							env_capacity;
	char						working_directory[PATH_MAX];

	int							heredoc_index;
	int							heredoc_fd;
	char						heredoc_delim[MAX_HEREDOCS][256];
	struct sigaction			saved_sigint;
	struct sigaction			saved_sigquit;
	struct sigaction			saved_sigterm;

	t_arena						*arena;
	t_arena						*frame_arena;
	t_arena						*active_arena;
}								t_shell;

// lexer data
typedef enum e_token_type
{
	END = 0,
	SQUOTE,
	DQUOTE,
	WORD,
	HEREDOC,
	REDIRECT_IN,
	REDIRECT_OUT,
	PIPE,
	APPEND,
	DOLLAR,
	INVALID,
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	size_t						content_len;
	bool						wp;
	char						*content;
	struct s_token				*next;
	struct s_token				*prev;
}								t_token;

typedef struct s_lexer
{
	char						*content;
	size_t						content_len;
	size_t						cursor;
	size_t						line;
}								t_lexer;

typedef void					(*t_builtin)(t_shell *shell, t_cmd_table *cmd);

// parser data
typedef enum e_cmd_type
{
	BUILTIN_CD,
	BUILTIN_ECHO,
	BUILTIN_EXIT,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_COUNT,
	EXTERNAL,
}								t_cmd_type;

typedef enum e_cmd_pos
{
	FIRST,
	MID,
	LAST
}								t_cmd_pos;

typedef struct s_da
{
	char						**items;
	size_t						count;
	size_t						cap;
}								t_da;

typedef struct s_str
{
	char						*str;
	size_t						len;
	size_t						cap;
}								t_str;

typedef struct s_cmd
{
	t_da						*cmd_da;
	t_cmd_type					cmd_type;
	t_cmd_pos					cmd_pos;
	int							fd_in;
	int							fd_out;
	char						*heredoc_delim;
	int							heredoc_index;
	struct s_cmd				*next;
	struct s_cmd				*prev;
}								t_cmd_table;

// heredoc data
typedef struct s_buf
{
	char						*data;
	size_t						len;
	size_t						cap;
	t_shell						*sh;
}								t_buf;

void							init_signals(t_shell *shell);
void							init_shell(char **av, char **env,
									t_shell *shell);
void							run_shell(t_shell *shell);
void							handle_signal(t_shell *shell, int sig);

// frame arena helpers
void							shell_begin_frame(t_shell *shell);
void							shell_end_frame(t_shell *shell);
t_arena							*sh_work_arena(t_shell *shell);

void							set_env_var(t_shell *shell, char *key,
									char *value);

char							*expand_dollar_variable(t_shell *shell,
									const char *var_name);

// heredoc
int								read_heredoc(t_shell *shell);
char							*expand_heredoc_line(t_shell *sh,
									const char *line);
void							leave_heredoc(t_shell *sh,
									struct sigaction *old_int);
void							heredoc_child(t_shell *sh, int write_fd,
									char *delim);
void							enter_heredoc(t_shell *sh,
									struct sigaction *old_int);
void							buf_puts(t_buf *b, const char *s);
void							buf_putc(t_buf *b, char c);
void							buf_grow(t_buf *b, size_t need);
void							buf_init(t_buf *b, t_shell *sh);

char							*get_env_var(t_shell *shell, char *key);
void							unset_env_var(t_shell *shell, char *key);
void							init_shell_env(t_shell *shell, char **av);
void							update_pwd_env(t_shell *shell, char *old_pwd,
									char *new_pwd);
void							update_last_command(t_shell *shell,
									char *command);

void							builtin_env(t_shell *shell, t_cmd_table *cmd);
void							builtin_echo(t_shell *shell, t_cmd_table *cmd);
void							builtin_exit(t_shell *shell, t_cmd_table *cmd);
void							builtin_pwd(t_shell *shell, t_cmd_table *cmd);
void							builtin_cd(t_shell *shell, t_cmd_table *cmd);
void							builtin_export(t_shell *shell,
									t_cmd_table *cmd);
void							builtin_unset(t_shell *shell, t_cmd_table *cmd);
char							*find_executable_path(char *cmd, char **env);

// parser prototypes
t_da							*da_cmd_init(t_arena *arena, size_t cap);
void							parser_da_append(char **da, char *str);
void							da_append(t_arena *arena, t_da *da, char *item);
t_str							*str_init(t_arena *arena, size_t cap);
void							str_append(t_arena *arena, t_str *str, char *item);
t_cmd_table						*parser_cmd_build_one(t_shell *shell,
									t_token *token);
t_cmd_table						*parser_cmd_build_many(t_shell *shell,
									t_token *token);
bool							parser_is_syntax_correct(t_token *token);
void							parser_cmd_type(t_shell *shell,
									t_cmd_table *cmd, t_token *token);
bool							parser_is_syntax_correct(t_token *token);
void							parser_cmd_type(t_shell *shell,
									t_cmd_table *cmd, t_token *token);
void							strip_delimiter(t_shell *shell, t_token *token);
bool							handle_token(t_shell *shell, t_cmd_table *cmd,
									t_token **tok, int *first);
bool							handle_word(t_shell *sh, t_cmd_table *cmd,
									t_token **tok, int *first);
char							*parser_expand_dollar(t_shell *shell,
									char *str);

// lexer prototypes
t_token							*build_token(t_arena *arena, t_token token);
t_token							*build_token_list(t_shell *shell, t_lexer *l);
t_lexer							build_lexer(char *content);
t_token							get_next_token(t_lexer *l);
t_token_type					get_token_type(const char c);
bool							is_operator(const char c);
bool							trim_left(t_lexer *l);
t_token							lexer_handle_append(t_lexer *l);
t_token							lexer_handle_heredoc(t_lexer *l);
t_token							lexer_handle_word(t_lexer *l, bool wp);
t_token							lexer_handle_squote(t_lexer *l, bool wp);
t_token							lexer_handle_dquote(t_lexer *l, bool wp);
t_token							lexer_handle_other_token(t_lexer *l);
bool							is_quote(const char c);

// exec prototypes
char							*exec_get_binary_path(char *cmd, char **env);
char							*exec_copy_bin_path(t_shell *shell, char *cmd);
void							exec_pipe(t_shell *shell);
void							exec_no_pipe(t_shell *shell);
pid_t							exec_pipeline(t_shell *shell, t_cmd_table *cmd);
void							exec_prep(t_cmd_table *cmd,
									t_pipe_line *pipeline);
void							close_pipe(t_pipe_line *pipe);

// execution
void							builtin_select(t_shell *shell,
									t_cmd_table *cmd);
#endif // MINISHELL_H
