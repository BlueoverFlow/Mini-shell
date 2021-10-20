/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 11:04:30 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 09:32:55 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*ft_getenv(t_data *data, char *var)
{
	t_list	*tmp;

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

static BOOL	compare_files(t_data *data, DIR *dir, char *prototype, char *tree)
{
	struct dirent	*list;

	list = readdir(dir);
	while (list)
	{
		if (!ft_strcmp(prototype, list->d_name))
		{
			closedir(dir);
			data->executable = ft_strjoin_and_free_s1
				(ft_strjoin(tree, "/"), prototype);
			return (TRUE);
		}
		list = readdir(dir);
	}
	data->executable = NULL;
	closedir(dir);
	return (FALSE);
}

BOOL	file_search_using_path_var(t_data *data, char *prototype)
{
	char	**tree;
	char	*path;
	DIR		*dir;
	int		i;

	path = ft_getenv(data, "PATH");
	if (!path)
	{
		data->err_path_env = TRUE;
		return (ERROR);
	}
	tree = ft_split(path, ':');
	i = -1;
	while (tree[++i])
	{
		dir = opendir(tree[i]);
		if (!dir)
			break ;
		if (compare_files(data, dir, prototype, tree[i]))
			return (TRUE);
	}
	free_2d(tree);
	return (FALSE);
}
