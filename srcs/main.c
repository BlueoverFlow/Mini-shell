/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 19:04:37 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void _init(t_data *data)
{
	data->commands = NULL;
	data->piped = NULL;
	data->fields = NULL;
	data->garbage = NULL;
}

int main()
{
	t_data data;
	t_list *cmd_tmp;

	data.line = NULL;
	while (READ)
	{
		_init(&data);
		if (!data_tree(&data))
			continue ;
		print_lines(data);			// just for debug
		/* execution */
		/* code */
	}
	return (0);
}