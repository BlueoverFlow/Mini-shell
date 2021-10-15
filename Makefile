# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/13 08:14:39 by ael-mezz          #+#    #+#              #
#    Updated: 2021/10/15 11:05:43 by ael-mezz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c								\
		utils/lst_utils.c					\
		utils/utils.c						\
		parsing/parsing.c					\
		parsing/expansions.c				\
		execution/execution.c				\
		execution/scan_command.c			\
		execution/find_executable.c			\
		execution/builtins/builtins.c		\
		execution/builtins/echo.c			\
		execution/builtins/env.c			\
		execution/builtins/export.c			\
		execution/builtins/cd.c				\
		execution/builtins/unset.c

LIBFT =	libft/libft.a

FLAGS = -g -lreadline -ledit #-Wall -Werror -Wextra -fsanitize=address

SRCS_PATH =	./srcs

CC = gcc

RM = rm -rf

OBJ = $(SRCS:.c=.o)

OBJ_DIR = mv *.o srcs

HEADER = headers/minishell.h

SRCS := $(addprefix $(SRCS_PATH)/, $(SRCS))

all: $(SRCS) $(HEADER)
	@make -C libft && make bonus -C libft
	@$(CC) $(SRCS) $(LIBFT) $(FLAGS) -o minishell
	@./minishell

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

re: fclean all
