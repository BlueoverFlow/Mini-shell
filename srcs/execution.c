/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/17 17:34:14 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void command_name(t_data *data, t_list *prototype)
{
	prototype->content = expand_unquoted_token(data, prototype->content);
	prototype->content = expand_in_double_quote(data, prototype->content);
	// prototype->content = expand_envirenment_var(prototype->content);
	prototype->content = expand_in_single_quote(data, prototype->content);
}

int execute(t_data *data)
{
	if (data->commands)
	{
		data->piped = data->commands->content;
		if (data->piped)
		{
			data->branch = data->piped->content;
			data->file = data->branch->content_2;
			data->prototype = data->branch->content_2;
			if (data->prototype)
			{
				command_name(data, data->prototype);
				printf("command name:|%s|\n", data->prototype->content);
				// is_builtin(data->prototype);
			}
		}
	}
	return (1);
}