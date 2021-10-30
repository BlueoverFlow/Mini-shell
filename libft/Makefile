# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/21 12:57:37 by ael-mezz          #+#    #+#              #
#    Updated: 2021/10/30 18:26:57 by ael-mezz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft

SRCS = 	ft_strcmp.c				\
		ft_free.c				\
		ft_atoi.c				\
		ft_bzero.c				\
		ft_isalnum.c			\
		ft_isalpha.c			\
		ft_isascii.c			\
		ft_isdigit.c			\
		ft_isprint.c			\
		ft_memccpy.c			\
		ft_memchr.c				\
		ft_memcmp.c				\
		ft_memcpy.c				\
		ft_memmove.c			\
		ft_memset.c				\
		ft_strchr.c				\
	   	ft_strlcat.c			\
	   	ft_strlcpy.c			\
	   	ft_strlen.c				\
	   	ft_strncmp.c			\
	   	ft_strnstr.c			\
	   	ft_strrchr.c			\
	   	ft_tolower.c			\
	   	ft_toupper.c			\
	   	ft_calloc.c				\
	   	ft_strdup.c				\
	   	ft_substr.c				\
	   	ft_strjoin.c			\
	   	ft_strtrim.c			\
	   	ft_split.c				\
	   	ft_itoa.c				\
	   	ft_strmapi.c			\
	   	ft_putchar_fd.c			\
	   	ft_putstr_fd.c			\
	   	ft_putendl_fd.c			\
	   	ft_putnbr_fd.c			\
	   	ft_strcpy.c				\
		ft_strcat.c				\
		ft_ptoa.c				\
		ft_memalloc.c			\
		ft_strnchar.c			\
		ft_strlen_2d.c			\
		free_2d.c				\
		ft_strcmp.c

BONUS_SRCS =	ft_lstnew_bonus.c			\
				ft_lstadd_front_bonus.c		\
				ft_lstsize_bonus.c			\
				ft_lstlast_bonus.c			\
				ft_lstadd_back_bonus.c		\
				ft_lstdelone_bonus.c		\
				ft_lstclear_bonus.c			\
				ft_lstiter_bonus.c			\
				ft_lstmap_bonus.c

SRCS_PATH =	./srcs

SRCS := $(addprefix $(SRCS_PATH)/, $(SRCS))

BONUS_SRCS := $(addprefix $(SRCS_PATH)/, $(BONUS_SRCS))

FLAGS = -Wall -Werror -Wextra -c

CC = gcc

RM = rm -f

OBJ_DIR = mv *.o srcs

OBJ = $(SRCS:.c=.o)

HEADER = includes/libft.h

BONUS_OBJ = $(BONUS_SRCS:.c=.o)

$(NAME): $(SRCS) $(HEADER)
	@$(CC) $(FLAGS) $(SRCS)
	@$(OBJ_DIR)
	@ar -rc $(NAME).a $(OBJ)
	@ranlib $(NAME).a

bonus: $(BONUS_SRCS) $(HEADER)
	@$(CC) $(FLAGS) $(BONUS_SRCS)
	@$(OBJ_DIR)
	@ar -rc $(NAME).a $(BONUS_OBJ)
	@ranlib $(NAME).a

all: $(NAME) bonus

clean:
	@$(RM) srcs/*.o

fclean: clean
	@$(RM) $(NAME)

re: fclean all

rebonus: fclean bonus
