/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@sudent.1337.ma>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/19 12:54:27 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void command_name_to_lower_case(t_list *prototype)
{
	int j;

	j = -1;
	while (((char *)prototype->content)[++j])
		((char *)prototype->content)[j] = ft_tolower(((char *)prototype->content)[j]);
}

static void expand_prototype(t_data *data, t_list *prototype)
{
	if (!prototype)
		return ;
	prototype->content = expand_token(data, prototype->content);

	/* still need to expand envirenment variable */

	if (ft_strcmp(prototype->content, "export"))
		expand_prototype(data, prototype->next);
}

int execute(t_data *data)
{
	while (data->piped)
	{
		data->branch = data->piped->content;
		data->file = data->branch->content_2;
		data->prototype = data->branch->content_2;
		if (data->prototype)
		{
			command_name_to_lower_case(data->prototype);
			expand_prototype(data, data->prototype);
			if (is_builtin(data) == ERROR)
				return (ERROR);
			/* execute the non-builtin commands */
		}
		data->piped = data->piped->next;
	}
	return (1);
}