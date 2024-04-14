
NAME		=	ft_nm

SRCS		=	$(wildcard srcs/*.c) $(wildcard srcs/*/*.c)

INCL_DIR	=	-I./includes -I./libs/libft

CC			=	gcc

CFLAGS		=	-Wall -Wextra -Werror -g3

OBJS_DIR	=	.objs

OBJS 		= 	$(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:%.c=%.o)))

DEP			=	$(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:%.c=%.d)))

$(OBJS_DIR)	: 
				@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o	: 	srcs/%.c
					@printf "\033[0;33mGenerating nm object... %-38.38s \r" $@
					@$(CC) $(CFLAGS) -c $< -o $@ -MMD $(INCL_DIR)
$(OBJS_DIR)/%.o	: 	srcs/32/%.c
					@printf "\033[0;33mGenerating nm object... %-38.38s \r" $@
					@$(CC) $(CFLAGS) -c $< -o $@ -MMD $(INCL_DIR)
$(OBJS_DIR)/%.o	: 	srcs/64/%.c
					@printf "\033[0;33mGenerating nm object... %-38.38s \r" $@
					@$(CC) $(CFLAGS) -c $< -o $@ -MMD $(INCL_DIR)

all	: $(NAME)

$(NAME)	: 			$(OBJS_DIR) $(OBJS)
				@$(MAKE) --no-print-directory -C libs/libft
		      		@$(CC) $(CFLAGS) $(OBJS) libs/libft/libft.a -o $(NAME) 
					@echo "\033[1;32m\nnm: Done!\033[0m"

clean	:
		      		@rm -f $(OBJS)
					@rm -rf $(OBJS_DIR)
					@echo "\033[1;31mObject cleaned!\033[0m"

fclean	:
					@rm -f $(OBJS)
					@rm -rf $(OBJS_DIR)
					@rm -f $(NAME)
					@echo "\033[1;31mProgram and object cleaned!\033[0m"

re		:			fclean all


.PHONY			:	all clean fclean re

-include $(DEP)
