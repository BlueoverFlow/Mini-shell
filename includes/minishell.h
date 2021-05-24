/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:35 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/24 13:04:00 by ael-mezz         ###   ########.fr       */
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
#define QUOTED_FIELD ((data->field[0] == '\'' || data->field[0] == '\"'))
#define NEED_MERGE ((data->field[0] != blanks[0] && data->field[0] != blanks[1]) && ft_lstsize(data->tokens) > 0)
#define NEXT_IS_UNCLOSED (data->pos[0] != ERROR && data->pos[1] == ERROR)
#define NO_MERGE (data->sub_field[++l] || data->field[0] == blanks[0] || data->field[0] == blanks[1])

typedef struct s_data
{
/*
linked lists
*/
	t_list *lines;
	t_list *commands;
	t_list *options;
	t_list *tokens;
	t_list *garbage;
/*
parser elems
*/
	char *input;
	BOOL is_quoted;
	char *field;
	int pos[2];
	char **sub_field;
	BOOL is_one_token;
	BOOL is_separated;
}				t_data;

//========================================================================================================
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
int		to_tokens(t_data *data);
void	parser(t_data *data, int i);

/*
quotes.c
*/
int to_tokens(t_data *data);

/*
expansions.c
*/
void expansions(t_data *data);

/*
utils.c
*/
void out(int code);
/*
special_char.c
*/
int is_backslashed(int i, char *str);
/*
utils_2.c
*/
char	**ft_split_blanks(char const *s);
//========================================================================================================

#endif