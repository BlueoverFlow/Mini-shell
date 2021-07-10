/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/07/10 09:55:12 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	command_name_to_lower_case(t_data *data)
{
	char	*tmp;
	int		j;

	j = -1;
	tmp = ft_strdup(data->prototype->content);
	while (((char *)data->prototype->content)[++j])
		((char *)data->prototype->content)[j] = ft_tolower(((char *)data->prototype->content)[j]);
	if (!ft_strcmp(data->prototype->content, "export") || \
		!ft_strcmp(data->prototype->content, "cd") \
		|| !ft_strcmp(data->prototype->content, "exit"))
		data->prototype->content = tmp;
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
	prototype->content = expand_env_var(data, prototype->content);
	expand_prototype(data, prototype->next);
}

static void	free_data(t_data data)
{
	free(data.input);
}

int	execute(t_data *data)
{
	char	**prototype;

	while (data->piped)
	{
		data->branch = data->piped->content;
		data->file = data->branch->content_2;
		data->prototype = data->branch->content_2;
		if (data->prototype)
		{
			command_name_to_lower_case(data);
			expand_prototype(data, data->prototype);
			prototype = lst_to_table(data->prototype);
			int ret = is_builtin(data, prototype);
			if (ret == -1)
				return (ERROR);
			/* execute the non-builtin commands */
			else if (ret == 0)
			{
				pid_t h;
				h = fork();
				if (h == 0)
				{
					binarycmd(*prototype);
					printf("%s: %s: command not found\n", "minishell", data->input);
				}
				wait(NULL);
			}
			free_data(*data);
		}
		data->piped = data->piped->next;
	}
	return (0);
}
