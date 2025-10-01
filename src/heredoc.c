/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:05:20 by jyniemit          #+#    #+#             */
/*   Updated: 2025/10/01 14:56:26 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int extract_var_name(const char *line, int start)
{
    int i = start;
    if (line[i] == '?')
        return i + 1;
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
        i++;
    return i;
}

static int    process_dollar_expansion(t_shell *shell, const char *line, int i, char *result)
{
    int        var_start;
    int        var_end;
    char    *var_name;
    char    *expanded;

    i++;
    var_start = i;
    var_end = extract_var_name(line, i);
    if (var_end > var_start)
    {
        var_name = arena_alloc(shell->arena, (var_end - var_start) + 1);
        ft_strlcpy(var_name, &line[var_start], (var_end - var_start) + 1);
        expanded = expand_dollar_variable(shell, var_name);
        ft_strlcat(result, expanded, 4096);
        return (var_end);
    }
    ft_strlcat(result, "$", 4096);
    return (i);
}

static void    append_single_char(char *result, char c)
{
    char    temp[2];

    temp[0] = c;
    temp[1] = '\0';
    ft_strlcat(result, temp, 4096);
}

static char    *expand_heredoc_line(t_shell *shell, const char *line)
{
    char    *result;
    size_t    line_len;
    int        i;
    int        next_i;

    line_len = ft_strlen(line);
    result = arena_alloc(shell->arena, 4096);
    result[0] = '\0';
    i = 0;
    while (line[i])
    {
        if (line[i] == '$')
        {
            next_i = process_dollar_expansion(shell, line, i, result);
            i = next_i;
        }
        else
        {
            append_single_char(result, line[i]);
            i++;
        }
    }
    return (result);
}


int read_heredoc(t_shell *shell)
{
    char tmp_name[32];
    char *line;
    char *delimiter;
    int fd;
	int	heredoc_index;
	char	*processed_line;

	heredoc_index = shell->heredoc_index;
	delimiter = shell->heredoc_delim[heredoc_index];
    ft_strlcpy(tmp_name, "/tmp/ms_", sizeof(tmp_name));
    tmp_name[8] = '0' + heredoc_index;
    tmp_name[9] = '\0';
    fd = open(tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd == -1) return -1;
    while ((line = readline("> "))) 
	{
		if (!line)
			break;
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
            && ft_strlen(line) == ft_strlen(delimiter)) 
		{
            free(line);
            break;
        }
		if (shell->state & HEREDOC_EXPAND)
        {
			processed_line = expand_heredoc_line(shell, line);
			write(fd, processed_line, ft_strlen(processed_line));
        }
		else
			write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    fd = open(tmp_name, O_RDONLY);
    unlink(tmp_name);
    return fd;
}
