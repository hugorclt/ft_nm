SRCS			=	ft_strncmp.c ft_strdup.c ft_strlen.c ft_utoa_base.c

OBJ			=	$(SRCS:.c=.o)

NAME		=	libft.a

all			:	$(NAME)

$(NAME)		:	$(OBJ)
				@ar rc $(NAME) $(OBJ) 
				@ranlib $(NAME)
				@echo "\033[1;32m\nLibft: done!\033[0m"
				
%.o			:	%.c
				@printf "\033[0;33mGenerating libft object... %-38.38s \r" $@
				@gcc -Wall -Wextra -Werror -c $< -o $@

clean		:
				@/bin/rm -f $(OBJ)

fclean		:	clean
				@/bin/rm -f $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re
