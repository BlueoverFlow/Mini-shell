# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/13 08:14:39 by ael-mezz          #+#    #+#              #
#    Updated: 2021/05/17 18:34:49 by ael-mezz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS =	main.c			\
		lst_utils.c		\
		parcing.c		\
		utils_1.c		\
		expansions_1.c

LIBFT =	libft/libft.a

FLAGS =	-g #-fsanitize=address #-Wall -Werror -Wextra

SRCS_PATH =	./srcs

CC = gcc

RM = rm -f

OBJ = $(SRCS:.c=.o)

OBJ_DIR = mv *.o srcs

HEADER = includes/minishell.h

SRCS := $(addprefix $(SRCS_PATH)/, $(SRCS))

ops: $(SRCS) $(HEADER)
	@cd libft && make all && make bonus
	@$(CC) $(SRCS) $(LIBFT) $(FLAGS) -o minishell

clean:
	@echo "cleaning..."

fclean:	clean
	@$(RM) -r minishell minishell.dSYM
	@cd libft && make fclean

test:
	@$(CC) srcs/$(FILE).c $(LIBFT) $(FLAGS) -o $(FILE)
	@echo "...test...\n\n====================="
	@./$(FILE)

exec:
	@./minishell

re: fclean ops exec
