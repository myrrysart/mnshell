/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/03 11:57:30 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_buf
{
	char	*data;
	size_t	len;
	size_t	cap;
	t_shell	*sh;
}			t_buf;

static void	buf_init(t_buf *b, t_shell *sh)
{
	b->cap = 128;
	b->len = 0;
	b->sh = sh;
	b->data = arena_alloc(sh_work_arena(sh), b->cap);
	if (b->data)
		b->data[0] = '\0';
}

static void	buf_grow(t_buf *b, size_t need)
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

static void	buf_putc(t_buf *b, char c)
{
	buf_grow(b, 1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
}

static void	buf_puts(t_buf *b, const char *s)
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

static char	*expand_heredoc_line(t_shell *sh, const char *line)
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

static void	heredoc_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

static void	heredoc_child(t_shell *sh, int write_fd, char *delim)
{
	char				*line;
	int					expand;
	char				*proc;
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_DFL);
	expand = (sh->state & HEREDOC_EXPAND) != 0;
	while ((line = readline("> ")) != NULL)
	{
		if (ft_strlen(line) == ft_strlen(delim) && !ft_strncmp(line, delim,
				ft_strlen(delim)))
		{
			free(line);
			break ;
		}
		if (expand)
		{
			proc = expand_heredoc_line(sh, line);
			if (proc)
				write(write_fd, proc, ft_strlen(proc));
			else
				write(write_fd, line, ft_strlen(line));
		}
		else
			write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}

static void	enter_heredoc(t_shell *sh, struct sigaction *old_int)
{
	struct sigaction	ign;

	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	ign.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ign, old_int);
	sh->state |= IN_HEREDOC;
}

static void	leave_heredoc(t_shell *sh, struct sigaction *old_int)
{
	sh->state &= ~IN_HEREDOC;
	sigaction(SIGINT, old_int, NULL);
}

int	read_heredoc(t_shell *sh)
{
	int					pfd[2];
	pid_t				pid;
	int					st;
	struct sigaction	old_int;

	if (sh->heredoc_index > 8)
		return (-1);
	if (pipe(pfd) < 0)
		return (-1);
	enter_heredoc(sh, &old_int);
	pid = fork();
	if (pid < 0)
	{
		leave_heredoc(sh, &old_int);
		close(pfd[RD]);
		close(pfd[WR]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pfd[RD]);
		heredoc_child(sh, pfd[WR], sh->heredoc_delim[sh->heredoc_index]);
	}
	close(pfd[WR]);
	while (waitpid(pid, &st, 0) == -1 && errno == EINTR)
		;
	leave_heredoc(sh, &old_int);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 130)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		sh->code = EXIT_SIGINT;
		sh->command_line[0] = '\0';
		sh->state &= ~(EVALUATING | IN_HEREDOC | IN_SQUOTE | IN_DQUOTE | HAS_PIPE);
		close(pfd[RD]);
		return (-2);
	}
	if (!(WIFEXITED(st) && WEXITSTATUS(st) == 0))
	{
		close(pfd[RD]);
		return (-1);
	}
	return (pfd[RD]);
}
