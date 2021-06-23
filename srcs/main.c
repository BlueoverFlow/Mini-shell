/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/23 12:28:03 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void _init(t_data *data)
{
	data->piped = NULL;
	data->prototype = NULL;
	data->file = NULL;
	data->branch = NULL;
	data->word = NULL;
	data->quoting_state = UNQUOTED;
	data->passive = FALSE;
}

int main(int argc, char **argv, const char **envp)
{
	t_data data;
	t_list *cmd_tmp;

	data.garbage = NULL;
	data.envp = envp;
	while (READ)
	{
		_init(&data);
		if (parser(&data) == ERROR)
			continue ;
		if (execute(&data) == ERROR)
			continue ;
		/* code */
	}
	return (0);
}