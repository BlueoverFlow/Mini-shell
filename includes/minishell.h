/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:35 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/13 16:55:55 by ael-mezz         ###   ########.fr       */
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
#define STD_OUTPUT 1
#define STD_INPUT 0
#define STD_APPENDED_OUTPUT 11
#define PROMPT "minishell$"
#define SNG_QUT 1
#define DBL_QUT 2
#define UNQUOTED 0
#define READ (write(STDERR_FILENO, PROMPT, 11) && get_next_line(STDIN_FILENO, &data.input) > 0 && ft_strcmp(data.input, "exit"))
#define QUOTED_FRAGMENT (data->input[i] == '\'' || data->input[i] == '\"')
#define NEED_MERGE ((data->field[0] != blanks[0] && data->field[0] != blanks[1]) && ft_lstsize(data->tokens) > 0)
#define NEXT_IS_UNCLOSED (data->pos[0] != ERROR && data->pos[1] == ERROR)

typedef struct s_list_2
{
    void			*content;
	void			*content_2;
	struct s_list_2	*next;
}               t_list_2;

typedef struct s_data
{
	t_list		*garbage;
	t_list		*commands;
	t_list		*piped;
	t_list		*prototype;
	t_list_2	*file;
	t_list_2	*branch;
	int			quoting_state;
	char		*input;
	char		*token;
	int			skip;
	int			pos[2];
	BOOL		is_separated;
	BOOL		merge;
}				t_data;

typedef struct s_var
{
    int     _error;
    int     _status;
}               t_var;


/*
main.c
*/
void _init(t_data *data);

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
int		tokens_analyser(t_data *data);
t_list_2 	*build_node(void *content, void *content_2);
t_list_2	*ft_lst2last(t_list_2 *lst);
int			ft_lst2size(t_list_2 *lst);
void		add_node(t_list_2 **alst, t_list_2 *new);

/*
parser.c
*/
int		data_tree(t_data *data);
int		parser(t_data *data);

/*
tokens.c
*/
int extract_branches(t_data *data);
/*
fields.c
*/

int		tokens_analyser(t_data *data);

/*
expansions.c
*/

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