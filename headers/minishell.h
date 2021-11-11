/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:35 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 15:26:02 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>

//macros
# define M_ARGERR "No such file or directory\n"
# define M_NOVALID "not a valid identifier\n"
# define M_UNFCMD "command not found\n"
# define M_FUNERR "function failure\n"
# define M_NOHOME "HOME not set\n"
# define M_STXERR "syntax error\n"
# define TRUE 10
# define FALSE 0
# define ERROR -1
# define REDIRECTED_OUTPUT 1
# define REDIRECTED_INPUT 0
# define HEREDOC 11
# define APPENDED_REDIRECTED_OUTPUT 22
# define PROMPT "minishell$ "
# define UNQUOTED 0
# define NORMAL_ERR 0
# define PERROR 5
# define EXPORT_ERR 6
# define UNSET_ERR 7
# define NO_FILE -2
# define BOOL int

typedef struct s_process
{
	pid_t	id;
}				t_process;

typedef struct s_h_d
{
	int			fd;
	char		*input;
	char		*file_name;
	t_list		*tmp;
}				t_h_d;

typedef struct s_file_data
{
	char		*path;
	char		*path_2;
	int			id;
}				t_file_data;

typedef struct s_command
{
	t_list		*prototype;
	t_list		*file;
}				t_command;

typedef struct s_info
{
	char	*var;
	char	*value;
}				t_info;

typedef struct	s_shell
{
	BOOL	parent;
	int		exit_status;
}				t_shell;


typedef struct s_data
{
	t_list			*piped_cmd;
	t_command		*command;
	t_list			*word;
	t_info			*info;
	t_list			*exported;
	t_list			*lst_child_id;
	t_process		*process;
	t_file_data		*file_data;
	BOOL			passive;
	BOOL			is_builtin;
	BOOL			err_path_env;
	BOOL			var_with_equals_sign;
	BOOL			infile;
	BOOL			outfile;
	pid_t			id;
	int				fd[4];
	int				end[2];
	int				argc;
	int				quoting_state;
	char			*input;
	char			*executable;
	char			**local_env;
	char			*document;
	char			**prototype;
	char			**argv;
	char *const		*envp;
}				t_data;

t_shell	g_shell;

//==================== utils ===================================

void		free_list(t_list **lst);
void		print_list(t_list *lst);
void		ft_dlstadd_back(t_list **alst, t_list *new);
t_list		*ft_dlstnew(void *content);
int			find_char(char *str, char c);
int			theres_atoken(char *fragment);
BOOL		is_redirection(t_data *data, char *str, int i);
BOOL		closed_quotes(char *input, int i);
char		*lst_to_word(t_list *lst);
int			syntax_checking(t_data *data, int i);
void		close_fds(t_data *data);
void		close_fds_and_wait(t_data *data);
void		build_env_vars(t_data *data, char *const	*envp);
void		export_print(t_data *data);
int			is_plus_sign(t_data *data, char *var, int i);
void		execve_errs(t_data data);
char		**env_array(t_data *data);
int			error_msg(t_data data, char *message, int exit_code, char *file);
void		insert_var(t_data *data, char *input);
void		execute_builtin(t_data *data, char	*cmd);
void		free_command_struct(t_data data);

//======== parsing ============================================

int			parser(t_data *data);
int			make_branch(t_data *data, char *fragment);
void		define_quoting_state(t_data *data, char *input, int i);
int			hundle_heredoc(t_data *data);
char		*expand_token(t_data *data, char *input);
char		*expand_env_vars(t_data *data, char *input);
int			hundle_redirection(t_data *data, char *fragment,
				char *token, int i);

//=========== execution ========================================

int			execute(t_data *data);
int			builtin(t_data *data);
int			echo(t_data data);
int			env(t_data data);
int			export(t_data *data);
int			cd(t_data *data);
int			pwd(t_data data);
int			unset(t_data *data);
void		build_env_vars(t_data *data, char *const	*envp);
void		scan_prototype(t_data *data);
int			file_search_using_path_var(t_data *data);
char		*ft_getenv(t_data data, char *var);
int			stream_source(t_data *data, int read_end, BOOL	simple_cmd);
void		sig_handler(int sig);

#endif