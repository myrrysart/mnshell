# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 12:37:53 by jyniemit          #+#    #+#              #
#    Updated: 2025/07/29 14:32:03 by jyniemit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFTDIR = ./libft/
LIBFT = $(LIBFTDIR)libft.a
INCLUDES = -I. -I./include -I$(LIBFTDIR)
LIBS = -lreadline $(LIBFT)

SRCDIR = src
OBJDIR = obj

SOURCES = \
		  main.c	\
		  repl.c	\
		  signals.c \
		  arena.c \

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
SRCFILES = $(addprefix $(SRCDIR)/, $(SOURCES))

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT)
	$(CC) $(OBJECTS) $(LIBS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
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

debug: fclean
	@make -C $(LIBFTDIR) debug
	$(MAKE) CFLAGS="$(CFLAGS) -g" all

re: fclean all

.PHONY: all clean fclean re debug
