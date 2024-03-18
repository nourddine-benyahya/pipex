SRC = Mandatory/envpath.c Mandatory/helper.c Mandatory/parsing.c \
		Mandatory/pipex.c Mandatory/process_funs.c

SRCB = bonus/envpath_bonus.c bonus/helper_bonus.c bonus/parsing_bonus.c \
		bonus/pipex_bonus.c bonus/process_funs_bonus.c bonus/here_doc_bonus.c

OBJ = $(SRC:.c=.o)

OBJB = $(SRCB:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./extra_libft
RM = rm -rf
NAME = pipex

LIBFT = extra_libft/libft.a

all : $(NAME)

bonus : $(OBJB)
	@make -C extra_libft all
	$(CC) $(CFLAGS) $(OBJB) $(LIBFT) -o $(NAME)

$(LIBFT):
	@make -C extra_libft all

$(NAME) : $(OBJ)
	@make -C extra_libft all
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	@make -C extra_libft clean
	$(RM) $(OBJ)
	$(RM) $(OBJB)

fclean : clean
	@make -C extra_libft fclean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re