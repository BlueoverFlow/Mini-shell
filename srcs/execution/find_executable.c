/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 11:04:30 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/15 12:18:21 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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

int file_search(t_data *data, char *prototype)
{
	char	**tree;
	char	*path;
	DIR		*dir;
	int		i;

	if (!(path = ft_getenv(data, "PATH")))
		return (error_msg(data, ft_strjoin(prototype, ": no such file or directory\n"), NORMAL_ERR));
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