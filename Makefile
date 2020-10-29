# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/14 12:07:00 by zlayine           #+#    #+#              #
#    Updated: 2020/10/29 18:57:20 by zlayine          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = srcs/utils.c \
		srcs/termcaps/config.c \
		srcs/termcaps/controls.c \
		srcs/termcaps/cursor.c \
		srcs/termcaps/display.c \
		srcs/termcaps/handlers.c \
		srcs/termcaps/history.c \
		srcs/termcaps/utils.c \
		srcs/exec_commands.c \
		srcs/handle_path.c \
		srcs/errors/errors.c \
		srcs/builtins/cd_builtin.c \
		srcs/builtins/echo_builtin.c \
		srcs/builtins/env_builtin.c \
		srcs/builtins/pwd_builtin.c \
		srcs/builtins/unset_builtin.c \
		srcs/builtins/export_builtin.c \
		srcs/utils/split.c \
		srcs/utils/debug.c \
		srcs/utils/free.c \
		srcs/parsing/main_parsing.c \
		srcs/parsing/helpers.c \
		srcs/parsing/var_manager.c \
		srcs/parsing/utils.c
		

LIBFT = ./libft

LIBFT_LIB = $(LIBFT)/libft.a

MINI_LIB = minishell.a

MINI_LIB_BONUS = minishell_bonus.a

OBJ = $(SRC:.c=.o)

INC = includes

MAIN = srcs/main.c

MAIN_BONUS = srcs/main_bonus.c

NORMAL = $(MINI_LIB)

BONUS = $(MINI_LIB_BONUS)

all: $(NORMAL)

bonus: $(BONUS)

$(LIBFT_LIB):
	@make -C $(LIBFT)

$(NORMAL): $(OBJ) $(MAIN) $(LIBFT_LIB) $(INC)/shell.h
	@ar rcs $(MINI_LIB) $(OBJ)
	@gcc -lncurses $(MAIN) $(MINI_LIB) $(LIBFT_LIB) -o $(NAME)
	@echo "SUCCESS! NO WALL WEXTRA WERROR ACTIVATED AND REMOVE DS STORE"

$(BONUS): $(OBJ) $(MAIN_BONUS) $(LIBFT_LIB) $(INC)/shell.h
	@ar rcs $(MINI_LIB_BONUS) $(OBJ)
	@gcc -lncurses $(MAIN_BONUS) $(MINI_LIB_BONUS) $(LIBFT_LIB) -o $(NAME)
	@echo "SUCCESS! NO WALL WEXTRA WERROR ACTIVATED AND REMOVE DS STORE"

%.o: %.c
	@gcc -c $< -o $@ -I $(INC)

clean:
	@make clean -C $(LIBFT)
	@rm -rf $(OBJ)

fclean: clean
	@make fclean -C $(LIBFT)
	@rm -rf $(MINI_LIB)
	@rm -rf $(MINI_LIB_BONUS)
	@rm -rf $(NAME)

re: fclean all

# @gcc -Wall -Wextra -Werror -c $< -o $@ -I $(INC)
