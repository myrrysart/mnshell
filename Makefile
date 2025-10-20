# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/03 12:12:38 by jyniemit          #+#    #+#              #
#    Updated: 2025/10/20 11:34:46 by jyniemit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory
NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFTDIR = ./libft/
LIBFT = $(LIBFTDIR)libft.a
INCLUDES = -I. -I./include -I$(LIBFTDIR)
LIBS = -lreadline $(LIBFT)
TESTDIR = ./test/

SRCDIR = src
OBJDIR = obj
INCLUDEDIR = include
ASMDIR = asm

HEADERS = minishell.h

SOURCES = \
	arena.c \
	arena_frame.c \
	builtin_cd.c \
	builtin_echo.c \
	builtin_exit.c \
	builtin_export.c \
	builtin_export_utils.c \
	builtin_pwd.c \
	builtin_unset.c \
	environment.c \
	environment_utilities.c \
	exec_pipeline.c \
	exec_others.c \
	exec_utils.c \
	exec_pipeline_prep.c \
	exec_pipeline_wait.c \
	exec_single_utils.c \
	heredoc.c \
	heredoc_utils.c \
	heredoc_child.c \
	heredoc_signals.c \
	heredoc_buf_utils.c \
	heredoc_flow.c \
	lexer.c \
	lexer_handle.c \
	lexer_handle_quote.c \
	lexer_utils.c \
	main.c \
	parser.c \
	parser_syntax_check.c \
	parser_utils.c \
	parser_word.c \
	parser_da.c \
	parser_handle.c \
	parser_handle_heredoc.c \
	parser_report.c \
	repl.c \
	repl_utils.c \
	shell_init.c \
	shell_environment.c \
	signals.c \
	signals_manager.c \
	signal_setups.c \
	str_builder.c \
	exec_redirs.c \
	child_cleanup.c \
	reporting.c \

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
SRCFILES = $(addprefix $(SRCDIR)/, $(SOURCES))
INCLUDEFILES = $(addprefix $(INCLUDEDIR)/, $(HEADERS))

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT)
	$(CC) $(OBJECTS) $(OBJMAIN) $(LIBS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDEFILES) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(ASMDIR)
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFTDIR) fclean

debug:
	$(MAKE) CFLAGS="$(CFLAGS) -g" all

re: fclean all

test: all
	@make -C $(TESTDIR)
	@./test/test

$(ASMDIR)/%.s: $(SRCDIR)/%.c $(INCLUDEFILES) | $(ASMDIR)
	$(CC) $(CFLAGS) -S -O0 $(INCLUDES) -c $< -o $@

$(ASMDIR):
	mkdir -p $(ASMDIR)

assembly: fclean $(SOURCES:%.c=$(ASMDIR)/%.s)
.PHONY: all clean fclean re debug test assembly
