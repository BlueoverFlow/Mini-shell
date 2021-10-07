/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/07 11:19:13 by ael-mezz         ###   ########.fr       */
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

int	execute(t_data *data)
{
	char	**prototype;
	pid_t	id;

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
			if (builtin(data, prototype) == ERROR)
				return (ERROR);
			if (!data->is_builtin)
			{
				id = fork();
				if (id == 0)
				{
					if (!run_executable(data, *prototype))
						printf("%s: %s: command not found\n", "minishell", prototype[0]);
				}
				wait(NULL);
			}
		}
		data->piped = data->piped->next;
	}
	return (0);
}
