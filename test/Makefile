MAKEFLAGS += --no-print-directory
NAME = test

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFTDIR = ../libft/
LIBFT = $(LIBFTDIR)libft.a
INCLUDES = -I. -I../include -I$(LIBFTDIR)

SRCDIR = ../src
OBJDIR = ../obj

SOURCES = test.c	\
		  arena.c	\
		  lexer.c	\
		  munit.c	\

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBFT) -o $(NAME)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
