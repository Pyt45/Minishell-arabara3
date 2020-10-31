# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/18 09:48:59 by zlayine           #+#    #+#              #
#    Updated: 2020/10/31 09:29:29 by zlayine          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = ft_atoi.c \
	ft_bzero.c \
	ft_calloc.c \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_itoa.c \
	ft_memccpy.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memset.c \
	ft_putchar_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_putstr_fd.c \
	ft_split.c \
	ft_strchr.c \
	ft_strdup.c \
	ft_strjoin.c \
	ft_strlcat.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strmapi.c \
	ft_strncmp.c \
	ft_strnstr.c \
	ft_strrchr.c \
	ft_strtrim.c \
	ft_substr.c \
	ft_tolower.c \
	ft_toupper.c \
	ft_pow.c \
	ft_putnbr.c \
	ft_putstr.c \
	ft_putchar.c \
	ft_hex_digit.c \
	ft_nblen.c \
	ft_nblen_un.c \
	ft_putnbr_un.c \
	ft_get_next_line.c \
	ft_get_newln_len.c \
	ft_new_str.c \
	ft_del.c \
	ft_strstr.c \
	ft_strcmp.c \
	ft_strcpy.c \
	ft_strcat.c \
	ft_arr_dup.c \
	ft_arr_len.c

BONUS_C = ft_lstadd_back_bonus.c \
	ft_lstadd_front_bonus.c \
	ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c \
	ft_lstiter_bonus.c \
	ft_lstlast_bonus.c \
	ft_lstmap_bonus.c \
	ft_lstnew_bonus.c \
	ft_lstsize_bonus.c
	
OBJ = $(SRC:.c=.o)
OBJ_B = $(BONUS_C:.c=.o)
CC = gcc
INC = libft.h
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ) $(INC)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)

bonus: all $(OBJ_B) $(INC)
	@ar rc $(NAME) $(OBJ_B)
	@ranlib $(NAME)

%.o: %.c
	@$(CC) $(FLAGS) -c $<
	
clean:
	@rm -f $(OBJ) $(OBJ_B)

fclean: clean
	@rm -f $(NAME)

re: fclean all