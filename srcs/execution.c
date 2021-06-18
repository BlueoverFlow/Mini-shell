/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/18 10:23:57 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void expand_prototype(t_data *data, t_list *prototype)
{
	if (!prototype)
		return ;
	prototype->content = expand_unquoted_token(data, prototype->content);
	prototype->content = expand_in_double_quote(data, prototype->content);
	// prototype->content = expand_envirenment_var(prototype->content);
	prototype->content = expand_in_single_quote(data, prototype->content);
	if (ft_strcmp(prototype->content, "export"))
		expand_prototype(data, prototype->next);
}

int execute(t_data *data)
{
	while (data->commands)
	{
		data->piped = data->commands->content;
		while (data->piped)
		{
			data->branch = data->piped->content;
			data->file = data->branch->content_2;
			data->prototype = data->branch->content_2;
			if (data->prototype)
			{
				expand_prototype(data, data->prototype);
				if (is_builtin(data) == ERROR)
					return (ERROR);
			}
			data->piped = data->piped->next;
		}
		data->commands = data->commands->next;
	}
	return (1);
}