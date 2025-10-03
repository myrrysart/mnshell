# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/03 12:12:38 by jyniemit          #+#    #+#              #
#    Updated: 2025/10/03 12:12:41 by jyniemit         ###   ########.fr        #
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

HEADERS = \
	  minishell.h \
	  arena.h \

SOURCES = main.c \
	repl.c \
	signals.c \
	arena.c \
	lexer.c \
	parser.c \
	environment.c \
	shell_environment.c \
	execution.c \
	builtin_echo.c \
	builtin_exit.c \
	builtin_pwd.c \
	builtin_cd.c \
	builtin_export.c \
	builtin_unset.c \
	execution_utils.c \
	exec_pipeline.c \
	heredoc.c \
	arena_frame.c \

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

debug: fclean
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
