SRC = Mandatory/envpath.c Mandatory/helper.c Mandatory/parsing.c Mandatory/pipex.c Mandatory/process_funs.c
OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./Extra-library

RM = rm -rf
NAME = pipex

LIBFT = Extra-library/libft.a

all : $(NAME)

$(LIBFT):
	make -C Extra-library all

$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	make -C Extra-library clean
	$(RM) $(OBJ)

fclean : clean
	make -C Extra-library fclean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
