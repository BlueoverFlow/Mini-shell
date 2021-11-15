/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 11:04:30 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/15 16:27:51 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*ft_getenv(t_data data, char *var)
{
	if (*var == '?')
		return (ft_itoa(g_shell.exit_status));
	while (data.exported)
	{
		data.info = data.exported->content;
		if (!(ft_strcmp(data.info->var, var)))
		{
			if (data.info->value && *(data.info->value))
				return (ft_strdup(data.info->value));
		}
		data.exported = data.exported->next;
	}
	return (NULL);
}

static BOOL	compare_files(t_data *data, DIR *dir, char *directory, char **tree)
{
	struct dirent	*list;

	list = readdir(dir);
	while (list)
	{
		if (!ft_strcmp(data->prototype[0], list->d_name))
		{
			closedir(dir);
			data->executable = ft_strjoin_and_free
				(ft_strjoin(directory, "/"), data->prototype[0], 1);
			free_2d(tree);
			return (TRUE);
		}
		list = readdir(dir);
	}
	data->executable = NULL;
	closedir(dir);
	return (FALSE);
}

BOOL	file_search_using_path_var(t_data *data)
{
	char	**tree;
	char	*path;
	DIR		*dir;
	int		i;

	data->err_path_env = FALSE;
	path = ft_getenv(*data, "PATH");
	if (!path)
	{
		data->err_path_env = TRUE;
		return (FALSE);
	}
	tree = ft_split(path, ':');
	free(path);
	i = -1;
	while (tree[++i])
	{
		dir = opendir(tree[i]);
		if (!dir)
			break ;
		if (compare_files(data, dir, tree[i], tree) == TRUE)
			return (TRUE);
	}
	free_2d(tree);
	return (FALSE);
}
