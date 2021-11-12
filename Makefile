# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/13 08:14:39 by ael-mezz          #+#    #+#              #
#    Updated: 2021/11/11 17:46:46 by ael-mezz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS =	minishell.c							\
		utils/lst_utils.c					\
		utils/utils.c						\
		utils/parsing_utils.c				\
		utils/execution_utils.c				\
		utils/execution_utils_2.c			\
		utils/export_utils.c				\
		parsing/parsing.c					\
		parsing/expansions.c				\
		parsing/expand_env_vars.c			\
		parsing/heredoc_parsing.c			\
		parsing/redirections.c				\
		execution/execution.c				\
		execution/streams.c					\
		execution/scan_prototype.c			\
		execution/find_executable.c			\
		execution/builtins/builtins.c		\
		execution/builtins/echo.c			\
		execution/builtins/env.c			\
		execution/builtins/export.c			\
		execution/builtins/cd.c				\
		execution/builtins/unset.c			\
		execution/builtins/pwd.c			\
		execution/builtins/exit.c			\
		execution/signals.c

SRCS_PATH =	./srcs

SRCS := $(addprefix $(SRCS_PATH)/, $(SRCS))

LIBFT =	libft/libft.a

FLAGS = -g -lreadline -L $(RDLINE_PATH)/lib -I $(RDLINE_PATH)/include #-Wall -Werror -Wextra #-fsanitize=address

CC = gcc

RM = rm -rf

HEADER = headers/minishell.h

$(NAME): $(SRCS) $(HEADER)
	@make all -C libft
	@$(CC) $(SRCS) $(LIBFT) $(FLAGS) -o $(NAME)

all: $(NAME)

clean:
	@echo "cleaning..."
	@make clean -C libft

fclean:	clean
	@$(RM) $(NAME) $(NAME).dSYM
	@make fclean -C libft

test:
	@$(CC) srcs/$(FILE).c $(LIBFT) $(FLAGS) -o $(FILE)
	@echo "...test...\n\n====================="
	@./$(FILE)

re: fclean all
