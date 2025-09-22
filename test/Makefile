MAKEFLAGS += --no-print-directory
NAME = test

CC = cc
CFLAGS = -Wall -Wextra -g
LIBFTDIR = ../libft/
LIBFT = $(LIBFTDIR)libft.a
INCLUDES = -I. -I../include -I$(LIBFTDIR)

SRCDIR = ../src/
OBJDIR = ../obj/

SRC = arena.c	\
	  lexer.c	\
	  parser.c	\
	  environment.c \
	  execution_utils.c

TEST = test.c	\
	   munit.c	\

OBJ = $(SRC:.c=.o)
TESTOBJ = $(TEST:.c=.o)

OBJECTS = $(addprefix $(OBJDIR), $(OBJ))

all: $(NAME)

$(NAME): $(OBJECTS) $(TESTOBJ)
	$(CC) $(OBJECTS) $(TESTOBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	cc $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm *.o
