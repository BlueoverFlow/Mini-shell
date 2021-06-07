/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:35 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 16:09:07 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

#include "../libft/includes/libft.h"

/*
macros
*/
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define BOOL int
#define PROMPT "minishell$"
#define SNG_QUT 5
#define DBL_QUT 6
#define UNQUOTED 7
#define READ (write(STDERR_FILENO, PROMPT, 11) && get_next_line(STDIN_FILENO, &data.input) > 0 && ft_strcmp(data.input, "exit"))
#define QUOTED_FIELD ((data->field[0] == '\'' || data->field[0] == '\"'))
#define NEED_MERGE ((data->field[0] != blanks[0] && data->field[0] != blanks[1]) && ft_lstsize(data->tokens) > 0)
#define NEXT_IS_UNCLOSED (data->pos[0] != ERROR && data->pos[1] == ERROR)

typedef struct s_data
{
	t_list	*line;
	t_list	*field_status;
	t_list	*options;
	t_list	*cmd;
	t_list	*tokens;
	t_list	*commands;
	t_list	*garbage;
	t_list	*piped;
	t_list	*tokens_head;
	char	*input;
	BOOL	is_quoted;
	char	*field;
	int		pos[2];
	char	**sub_field;
	BOOL	is_one_token;
	BOOL	is_separated;
}				t_data;

typedef struct s_var
{
    int     _error;
    int     _status;
}               t_var;

//========================================================================================================
/*
lst_utils.c
*/
void    free_list(t_list **lst);
void	print_list(t_list *lst);
t_list	*ft_lstprevious(t_list *lst);
void	ft_dlstadd_back(t_list **alst, t_list *new);
void	print_content_list(t_list *lst);
void	print_lines(t_data data);

/*
parser.c
*/
int		data_tree(t_data *data);
int		parser(t_data *data, int i, char *input);

/*
quotes.c
*/
int		to_tokens(t_data *data, char *input);

/*
expansions.c
*/
void analyse_tokens(t_data *data);

/*
utils.c
*/
void	out(int code, t_data data);
int		is_backslashed(int i, char *str);
char	**ft_split_input(char const *s, char *separator);

/*
is_commands.c
*/
void	commands(t_data *data);
void	piped(t_data *data);
//========================================================================================================

#endif