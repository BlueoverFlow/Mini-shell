/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:35 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/17 18:24:23 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

#include "../libft/includes/libft.h"

#define PROMPT "minishell$"
#define READ (write(STDERR_FILENO, PROMPT, 11) && get_next_line(STDIN_FILENO, &data.input) > 0 && ft_strcmp(data.input, "exit"))
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define BOOL int

typedef struct s_data
{
	t_list *lines;
	t_list *commands;
	t_list *options;
	t_list *tokens;
	t_list *garbage;
	char *input;
	int is_quoted;
	char *field;
	int nq;
}				t_data;

/*
lst_utils.c
*/
void    free_list(t_list **lst);
void	print_list(t_list *lst);
t_list	*ft_lstprevious(t_list *lst);
void	ft_dlstadd_back(t_list **alst, t_list *new);

/*
parser.c
*/
void	parser(t_data *data, int i);

/*
expansions.c
*/
void expansions(t_data *data);

/*
utils.c
*/
void out(int code);

#endif