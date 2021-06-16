/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/16 12:05:29 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void _init(t_data *data)
{
	data->commands = NULL;
	data->piped = NULL;
	data->prototype = NULL;
	data->file = NULL;
	data->branch = NULL;
	data->word = NULL;
	data->quoting_state = UNQUOTED;
}

int main()
{
	t_data data;
	t_list *cmd_tmp;

	data.garbage = NULL;
	while (READ)
	{
		_init(&data);
		if (parser(&data) == ERROR)
			continue ;
		while (data.commands)
		{
			data.piped = data.commands->content;
			while (data.piped)
			{
				data.branch = data.piped->content;
				data.prototype = data.branch->content_2;
				while (data.prototype)
				{
					printf("prototype:|%s|\n", data.prototype->content);
					data.prototype = data.prototype->next;
				}
				data.piped = data.piped->next;
				if (data.piped)
					puts("\t|\n\t|");
			}
			puts("=============================\n");
			data.commands = data.commands->next;
												// exit(0);
		}
		// print_lines(data);			// just for debug
		/* execution */
		/* code */
	}
	return (0);
}