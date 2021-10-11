/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/11 18:32:07 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	command_name_to_lower_case(t_data *data)
{
	char	*tmp;
	int		j;

	j = -1;
	tmp = ft_strdup(data->command->prototype->content);
	while (((char *)data->command->prototype->content)[++j])
		((char *)data->command->prototype->content)[j] = ft_tolower(((char *)data->command->prototype->content)[j]);
	if (!ft_strcmp(data->command->prototype->content, "export") || \
		!ft_strcmp(data->command->prototype->content, "cd") \
		|| !ft_strcmp(data->command->prototype->content, "exit"))
		data->command->prototype->content = tmp;
	data->cmd_name = tmp;
}

static char	**lst_to_table(t_list *lst)
{
	char	**table;
	int		i;

	table = malloc(sizeof(*table) * (ft_lstsize(lst) + 1));
	i = 0;
	while (lst)
	{
		table[i++] = ft_strdup(lst->content);
		lst = lst->next;
	}
	table[i] = NULL;
	return (table);
}

static void	expand_prototype(t_data *data, t_list *prototype)
{
	if (!prototype)
		return ;
	prototype->content = expand_token(data, prototype->content);
	prototype->content = expand_env_vars(data, prototype->content);
	expand_prototype(data, prototype->next);
}

static char	*ft_getenv(t_data *data, char *var)
{
	t_list *tmp;
	char	*value;
	char	*key;

	if (!data->exported)
		build_env_vars(data);
	tmp = data->exported;
	while (tmp)
	{
		data->info = tmp->content;
		if (!(ft_strcmp(data->info->var, var)))
		{
			if (data->info->value && *(data->info->value))
				return (data->info->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

static int compare_files(t_data *data, DIR *dir, char *prototype, char *tree)
{
	struct dirent	*list;

	list = readdir(dir);
	while (list)
	{
		if (!ft_strcmp(prototype, list->d_name))
		{
			closedir(dir);		
			data->executable = ft_strjoin_and_free(ft_strjoin_and_free(tree, "/"), prototype);		
			return (1);
		}
		list = readdir(dir);
	}
	data->executable = NULL;
	closedir(dir);
	return (0);
}

static int file_search(t_data *data, char *prototype)
{
	char	**tree;
	char	*path;
	DIR		*dir;
	int		i;

	if (!(path = ft_getenv(data, "PATH")))
		return (error_msg(data, ft_strjoin(prototype, ": No such file or directory\n"), NORMAL_ERR));
	tree = ft_split(path, ':');
	i = -1;
	while (tree[++i])
	{
		dir = opendir(tree[i]);
		if (compare_files(data, dir, prototype, tree[i]))
			return (1);
	}
	free_2d(tree);
	return (error_msg(data, ft_strjoin(prototype, ": command not found\n"), NORMAL_ERR));
}

static int give_input(t_data *data)
{
	t_list *tmp;
	int		fd;

	fd = -2;
	if (data->outfile)
	{
		tmp = data->command->file;
		while (tmp && data->outfile)
		{
			data->file_data = tmp->content;
			if (data->file_data->id == REDIRECTED_OUTPUT)
			{
				if ((fd = open(data->file_data->path, O_CREAT | O_WRONLY | O_APPEND | O_TRUNC, 666) < 3))
				{
					return(error_msg(data, NULL, PERROR));
				}
				if (data->outfile-- > 1)
					close(fd);
			}
			else if (data->file_data->id == APPENDED_REDIRECTED_OUTPUT)
			{
				if ((fd = open(data->file_data->path, O_CREAT | O_WRONLY | O_APPEND, 666) < 3))
				{
					return(error_msg(data, NULL, PERROR));
				}
				if (data->outfile-- > 1)
					close(fd);
			}
		}
	}
	return (fd);
}

static int take_ouput(t_data *data)
{
	t_list *tmp;
	int		fd; 

	fd = -2;
	if (data->infile)
	{
		tmp = data->command->file;
		while (tmp && data->infile)
		{
			data->file_data = tmp->content;
			if (data->file_data->id == REDIRECTED_INPUT)
			{
				if ((fd = open(data->file_data->path, O_RDONLY) < 3))
				{
					return(error_msg(data, NULL, PERROR));
				}
				if (data->infile-- > 1)
					close(fd);
			}
			else if (data->file_data->id == HEREDOC)
			{
				if ((fd = open(data->document, O_RDONLY) < 3))
				{
					return(error_msg(data, NULL, PERROR));
				}
				if (data->infile-- > 1)
					close(fd);
			}
		}
	}
	return (fd);
}

static int	duplicate(t_data *data, int fd_in, int fd_out, int *ends)
{
	if (fd_in == ERROR || fd_out == ERROR)
		return (ERROR);
	if (fd_in == -2)
		fd_in = ends[0];
	if (fd_out == -2)
		fd_out = data->ends_2[1];
	if (data->piped_cmd->previous)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			return(error_msg(data, NULL, PERROR));
		}
	}
	if (data->piped_cmd->next)
	{
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			return(error_msg(data, NULL, PERROR));
		}
	}
	return (1);
}

static int check_the_ends(t_data *data)
{
	t_list *tmp;

	data->infile = FALSE;
	data->outfile = FALSE;
	if (data->command->file)
	{
		tmp = data->command->file;
		while (tmp)
		{
			data->file_data = tmp->content;
			if (data->file_data->id == HEREDOC || data->file_data->id == REDIRECTED_INPUT)
				data->infile++;
			if (data->file_data->id == APPENDED_REDIRECTED_OUTPUT || data->file_data->id == REDIRECTED_OUTPUT)
				data->outfile++;
			tmp = tmp->next;
		}
	}
	return (1);
}

static void close_fds(t_data *data, int *ends)
{
	close(ends[0]);
	close(ends[1]);
	close(data->ends_2[0]);
	close(data->ends_2[1]);
}

static int call_the_daughter(t_data *data, char **prototype, int *ends)
{
	check_the_ends(data);
	if (duplicate(data, take_ouput(data), give_input(data), ends) == ERROR
	|| builtin(data, prototype) == ERROR
	|| (!data->is_builtin && file_search(data, prototype[0]) == ERROR))
	{
		return (ERROR);
	}
	close_fds(data, ends);
	if (!data->is_builtin && execve(data->executable, prototype, NULL))
	{
		return (error_msg(data, NULL, PERROR));
	}
	return (1);
}

static int	pipe_and_fork(t_data *data, char **prototype)
{
	pid_t id;

	if (data->piped_cmd->next)
	{
		if (pipe(data->ends_2) == -1)
		{
			return(error_msg(data, NULL, PERROR));
		}
	}
	if ((id = fork()) == ERROR)
	{
		return(error_msg(data, NULL, PERROR));
	}
	return (id);
}

static int	sugar_dady(t_data *data, char **prototype, int *ends)
{
	pid_t	id;
	int		stat;
	t_list	*tmp;

	if (data->piped_cmd)
	{
		data->command = data->piped_cmd->content;
		command_name_to_lower_case(data);
		expand_prototype(data, data->command->prototype);
		prototype = lst_to_table(data->command->prototype);
		id = pipe_and_fork(data, prototype);
		if (id == 0)
			call_the_daughter(data, prototype, ends);
		waitpid(id, &stat, 0);
		if (WIFEXITED(stat))
			data->exit_status = WEXITSTATUS(stat);
		if (data->piped_cmd->next)
		{
			data->piped_cmd = data->piped_cmd->next;
			if (sugar_dady(data, prototype, data->ends_2) == ERROR)
			{
				return (ERROR);
			}
		}
		close_fds(data, ends);
	}
	return (1);
}

int	execute(t_data *data)
{
	char	**prototype;
	pid_t	id;
	int		ends[2];
	
	if (sugar_dady(data, prototype, ends) == ERROR)
		return (ERROR);
	return (1);
}
