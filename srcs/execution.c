/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/11 10:06:03 by ael-mezz         ###   ########.fr       */
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

// static char	*ft_getenv(t_data *data, char *var)
// {
// 	t_list *tmp;
// 	char	*value;
// 	char	*key;

// 	if (!data->exported)
// 		export(data, NULL);
// 	tmp = data->exported;
// 	while (data->exported)
// 	{
// 		if (find_value(data, var, &key))
// 		value = ft_substr(key, 2, ft_strlen(key) - 3);
// 		free(key);
// 		if (value && *value)
// 		{
// 			data->exported = tmp;
// 			return (value);
// 		}
// 		data->exported = data->exported->next;
// 	}
// 	data->exported = tmp;
// 	return (NULL);
// }

// static int compare_files(t_data *data, DIR *dir, char *prototype, char *tree)
// {
// 	struct dirent	*list;

// 	list = readdir(dir);
// 	while (list)
// 	{
// 		if (!ft_strcmp(prototype, list->d_name))
// 		{
// 			closedir(dir);
// 			data->executable = ft_strjoin(tree, "/");
// 			data->executable = ft_strjoin(data->executable, prototype);
// 			return (1);
// 		}
// 		list = readdir(dir);
// 	}
// 	data->executable = NULL;
// 	closedir(dir);
// 	return (0);
// }

// static int file_search(t_data *data, char *prototype)
// {
// 	char	*path;
// 	DIR		*dir;
// 	char	**tree;
// 	int		i;

// 	if (!(path = ft_getenv(data, "PATH")))
// 		return (error_msg(data, ft_strjoin(prototype, ": No such file or directory\n"), NORMAL_ERR));
// 	tree = ft_split(path, ':');
// 	i = -1;
// 	while (tree[++i])
// 	{
// 		dir = opendir(tree[i]);
// 		if (compare_files(data, dir, prototype, tree[i]))
// 			return (1);
// 	}
// 	free_2d(tree);
// 	return (0);
// }

// static int give_input(t_data *data, int *ends_2)
// {
// 	t_list *tmp;

// 	data->fd = -1;
// 	tmp = data->command->file;
// 	if (data->outfile)
// 	{
// 		while (tmp && data->infile)
// 		{
// 			data->file_data = tmp->content;
// 			if (data->file_data->id == REDIRECTED_OUTPUT)
// 			{
// 				if ((data->fd = open(data->file_data->path, O_CREAT | O_WRONLY | O_APPEND | O_TRUNC, 666) < 3))
// 				{
// 					return(error_msg(data, NULL, PERROR));
// 				}
// 				if (data->infile-- > 1)
// 					close(data->fd);
// 			}
// 			else if (data->file_data->id == APPENDED_REDIRECTED_OUTPUT)
// 			{
// 				if ((data->fd = open(data->file_data->path, O_CREAT | O_WRONLY | O_APPEND, 666) < 3))
// 				{
// 					return(error_msg(data, NULL, PERROR));
// 				}
// 				if (data->infile-- > 1)
// 					close(data->fd);
// 			}
// 		}
// 	}
// 	if (data->fd == -1)
// 	{
// 		data->fd = ends_2[1];
// 		if (!data->piped_cmd->next)
// 			data->fd = STDOUT_FILENO;
// 	}
// 	if (dup2(STDOUT_FILENO, data->fd) == -1)
// 	{
// 		return(error_msg(data, NULL, PERROR));
// 	}
// 	return (data->fd);
// }

// static int take_ouput(t_data *data, int *ends)
// {
// 	t_list *tmp;
// 	static int i = 0;

// 	data->fd = -1;
// 	tmp = data->command->file;
// 	if (data->infile)
// 	{
// 		close(ends[0]);
// 		while (tmp && data->infile)
// 		{
// 			data->file_data = tmp->content;
// 			if (data->file_data->id == REDIRECTED_INPUT)
// 			{
// 				if ((data->fd = open(data->file_data->path, O_RDONLY) < 3))
// 				{
// 					return(error_msg(data, NULL, PERROR));
// 				}
// 				if (data->infile-- > 1)
// 					close(data->fd);
// 			}
// 			else if (data->file_data->id == HEREDOC)
// 			{
// 				if ((data->fd = open(data->document, O_RDONLY) < 3))
// 				{
// 					return(error_msg(data, NULL, PERROR));
// 				}
// 				if (data->infile-- > 1)
// 					close(data->fd);
// 			}
// 		}
// 	}
// 	if (data->fd == -1)
// 		data->fd = ends[0];
// 	if (dup2(data->fd, STDIN_FILENO) == -1)
// 	{
// 		return(error_msg(data, NULL, PERROR));
// 	}
// 	i++;
// 	return (data->fd);
// }

// static int check_the_ends(t_data *data)
// {
// 	t_list *tmp;

// 	data->infile = FALSE;
// 	data->outfile = FALSE;
// 	if (data->command->file)
// 	{
// 		tmp = data->command->file;
// 		while (tmp)
// 		{
// 			data->file_data = tmp->content;
// 			if (data->file_data->id == HEREDOC || data->file_data->id == REDIRECTED_INPUT)
// 				data->infile++;
// 			if (data->file_data->id == APPENDED_REDIRECTED_OUTPUT || data->file_data->id == REDIRECTED_OUTPUT)
// 				data->outfile++;
// 			tmp = tmp->next;
// 		}
// 	}
// 	return (1);
// }

// static int	new_child(t_data *data, char **prototype, int *ends)
// {
// 	pid_t	id;
// 	int		ends_2[2];
// 	int		stat;
// 	if (data->piped_cmd)
// 	{
// 		data->command = data->piped_cmd->content;
// 		expand_prototype(data, data->command->prototype);
// 		prototype = lst_to_table(data->command->prototype);
// 		if (data->piped_cmd->next)
// 		{
// 			if (pipe(ends_2) == -1)
// 			{
// 				return (error_msg(data, ft_strjoin(prototype[0], ": command not found\n"), NORMAL_ERR));
// 			}
// 		}
// 		if (file_search(data, prototype[0]) == ERROR)
// 		{
// 			return (ERROR);
// 		}
// 		if (!data->executable)
// 		{
// 			return (error_msg(data, ft_strjoin(prototype[0], ": command not found\n"), NORMAL_ERR));
// 		}
// 		if ((id = fork()) == ERROR)
// 		{
// 			return(error_msg(data, NULL, PERROR));
// 		}
// 		fprintf(stderr, "%s\n", data->executable);
// 		fflush(stderr);
// 		check_the_ends(data);
// 		fprintf(stderr, "%d\n", data->infile);
// 		fprintf(stderr, "%d\n\n", data->outfile);
// 		take_ouput(data, ends);
// 		give_input(data, ends_2);
// 		if (id == 0)
// 		{
// 			if (execve(data->executable, prototype, NULL))
// 			{
// 				return (error_msg(data, NULL, PERROR));
// 			}
// 		}
// 		waitpid(id, &stat, 0);
// 		if (WIFEXITED(stat))
// 			data->exit_status = WEXITSTATUS(stat);
// 		if (!data->infile)
// 			close(ends[0]);
// 		if (data->piped_cmd->next)
// 		{
// 			data->piped_cmd = data->piped_cmd->next;
// 			if (new_child(data, prototype, ends_2) == ERROR)
// 			{
// 				return (ERROR);
// 			}
// 		}
// 	}
// 	return (1);
// }

int	execute(t_data *data)
{
	char	**prototype;
	pid_t	id;
	int		ends[2];
	
	while (data->piped_cmd)
	{
		data->command = data->piped_cmd->content;
		if (data->command->file)
			data->file_data = data->command->file->content;
		command_name_to_lower_case(data);
		expand_prototype(data, data->command->prototype);
		prototype = lst_to_table(data->command->prototype);
		if (builtin(data, prototype) == ERROR)
			return (ERROR);
		data->piped_cmd = data->piped_cmd->next;
	}
	return (1);
}
