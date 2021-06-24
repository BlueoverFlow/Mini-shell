/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:35 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/24 13:34:15 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "../libft/includes/libft.h"
# include <errno.h>
# include <sys/types.h>

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
#define STD_APPENDED_INPUT 22
#define PROMPT "minishell$ "
#define UNQUOTED 0
#define READ (write(STDERR_FILENO, PROMPT, 12) && get_next_line(STDIN_FILENO, &data.input) > 0 && ft_strcmp(data.input, "exit"))

typedef struct s_list_2
{
    void			*content;
	void			*content_2;
	struct s_list_2	*next;
}               t_list_2;

typedef struct s_data
{
	t_list		*garbage;
	t_list		*piped;
	t_list		*word;
	t_list		*prototype;
	t_list		*exported;
	t_list_2	*file;
	t_list_2	*branch;
	const char	**envp;
	int			quoting_state;
	int			exit_status;
	BOOL		passive;
	char		*input;
	char		*cmd_name;
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
int		tokens_analyser(t_data *data);
t_list_2 	*build_node(void *content, void *content_2);
t_list_2	*ft_lst2last(t_list_2 *lst);
int			ft_lst2size(t_list_2 *lst);
void		add_node(t_list_2 **alst, t_list_2 *new);
t_list *lst_elem(t_list *lst, int index);
t_list	*ft_dlstnew(void *content);
void	ft_dlst_delete_node(t_list *lst);

/*
parser.c
*/
int		parser(t_data *data);
int		make_branch(t_data *data, char *fragment);
void	define_quoting_state(t_data *data, char *input, int i);

/*
expansions.c
*/
char	*expand_token(t_data *data, char *input);

/*
utils.c
*/
int		out(t_data *data, char *exit_message, int code);
int		is_backslashed(int i, char *str);
int		find_char(char *str, char c);
char	**ft_split_input(char const *s, char *separator);
BOOL	quoted_fragment(char c);

/*
execution.c
*/
int execute(t_data *data);

/*
builtins.c
*/
int is_builtin(t_data *data, char **prototype);
int     echo(char **args);
int env(t_data *data, char **prototype);
int export(t_data *data, char **prototype);

//========================================================================================================

#endif