# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/13 08:14:39 by ael-mezz          #+#    #+#              #
#    Updated: 2021/06/29 12:10:37 by mlabrayj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c			\
		lst_utils.c		\
		parsing.c		\
		utils.c			\
		expansions.c	\
		execution.c		\
		builtins.c		\
		echo.c			\
		env.c			\
		export.c

LIBFT =	libft/libft.a

FLAGS =	-g -fsanitize=address #-Wall -Werror -Wextra

SRCS_PATH =	./srcs

CC = gcc

RM = rm -rf

OBJ = $(SRCS:.c=.o)

OBJ_DIR = mv *.o srcs

HEADER = includes/minishell.h

SRCS := $(addprefix $(SRCS_PATH)/, $(SRCS))

all: $(SRCS) $(HEADER)
	@make -C libft && make bonus -C libft
	@$(CC) $(SRCS) $(LIBFT) $(FLAGS) -o minishell

clean:
	@echo "cleaning..."
	@make clean -C libft

fclean:	clean
	@$(RM) minishell minishell.dSYM
	@make fclean -C libft

test:
	@$(CC) srcs/$(FILE).c $(LIBFT) $(FLAGS) -o $(FILE)
	@echo "...test...\n\n====================="
	@./$(FILE)

exec:
	@./minishell

re: fclean all exec
