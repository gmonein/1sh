NAME = minishell

CPP = clang

CFLAGS = -fsanitize=address -g3 #-Ofast -march=native -Wall -Wextra -Werror

DIR_LIB = srcs/libft/

HEADER = -I includes -I $(DIR_LIB)/includes/

INCLUDES = includes/minishell.h

LIBS = -ltermcap

SOURCES = auto_completion.c \
			cd.c \
			echo.c \
			env.c \
			exec_cmd.c \
			exit.c \
			main.c \
			parsing.c \
			pwd.c \
			realloc.c \
			setenv.c \
			termcaps.c \
			tools.c \
			unsetenv.c \
			move_cursor.c \
			insert_char.c

DIR_O = objs

DIR_S = srcs

SRCS = $(addprefix $(DIR_S)/,$(SOURCES))
OBJS = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: $(NAME)

${NAME}: $(DIR_O) $(OBJS)
	make -C $(DIR_LIB)
	${CPP} $(CFLAGS) -o $@ $(LIBS) $(OBJS) $(DIR_LIB)/libft.a

$(DIR_O)/%.o: $(DIR_S)/%.c $(INCLUDES)
	$(CPP) $(CFLAGS) $(HEADER) -c -o $@ $<

$(DIR_O):
	mkdir -p $(DIR_O)

clean:
	make clean -C $(DIR_LIB)
	rm -rf $(OBJS)
	rm -rf $(DIR_O)

fclean: clean
	make fclean -C $(DIR_LIB)
	rm -rf $(NAME)

re: fclean all
