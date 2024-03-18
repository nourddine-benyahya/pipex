SRC = Mandatory/envpath.c Mandatory/helper.c Mandatory/parsing.c \
		Mandatory/pipex.c Mandatory/process_funs.c

SRCB = bonus/envpath_bonus.c bonus/helper_bonus.c bonus/parsing_bonus.c \
		bonus/pipex_bonus.c bonus/process_funs_bonus.c bonus/here_doc_bonus.c

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./extra_libft
RM = rm -rf
HEADER = Mandatory/pipex.h
HEADERB = bonus/pipex_bonus.h
NAME = pipex
LIBFT = extra_libft/libft.a

all : $(NAME)

$(NAME) : $(SRC) $(LIBFT) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) $(LIBFT) -o $(NAME)

bonus : $(SRCB) $(LIBFT) $(HEADERB)
	$(CC) $(CFLAGS) $(SRCB) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C extra_libft all

clean :
	make -C extra_libft clean

fclean : clean
	make -C extra_libft fclean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean bonus fclean re