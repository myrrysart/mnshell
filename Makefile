# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#              #
#    Updated: 2025/08/27 12:54:15 by jyniemit         ###   ########.fr        #
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

HEADERS = \
		  minishell.h \
		  lexer.h \
		  arena.h \

SOURCES = main.c	\
		  repl.c	\
		  signals.c	\
		  arena.c	\
		  lexer.c	\
		  parser.c	\
		  environment.c \

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

.PHONY: all clean fclean re debug
