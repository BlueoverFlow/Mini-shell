/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@sudent.1337.ma>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/18 18:26:45 by ael-mezz         ###   ########.fr       */
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
	data->old_quoting_state = UNQUOTED;
	data->current_state = UNQUOTED;
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
		if (execute(&data) == ERROR)
			return (out(0, data));
		printf("\033[H\033[J");
		/* code */
	}
	return (0);
}