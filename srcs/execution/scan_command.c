/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 10:58:05 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/15 11:03:45 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	command_name_to_lower_case(t_data *data)
{
	char	*tmp;
	char	*command_name;
	int		j;

	j = -1;
	command_name = data->command->prototype->content;
	tmp = ft_strdup(command_name);
	while (command_name[++j])
		command_name[j] = ft_tolower(command_name[j]);
	if (!ft_strcmp(command_name, "export") || !ft_strcmp(command_name, "cd")
		|| !ft_strcmp(command_name, "exit"))
			data->command->prototype->content = tmp;
	else
		free(tmp);
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

char **scan_command(t_data *data)
{
	char **prototype;

	data->command = data->piped_cmd->content;
	command_name_to_lower_case(data);
	expand_prototype(data, data->command->prototype);
	prototype = lst_to_table(data->command->prototype);
	return (prototype);
}