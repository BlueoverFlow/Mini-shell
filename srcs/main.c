/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/04 15:49:17 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	global_init(t_data *data, const char **envp)
{
	data->exit_status = 0;
	data->garbage = NULL;
	data->envp = envp;
}

static void	_init(t_data *data)
{
	data->piped = NULL;
	data->prototype = NULL;
	data->file = NULL;
	data->branch = NULL;
	data->word = NULL;
	data->quoting_state = UNQUOTED;
	data->passive = FALSE;
}

int	main(int argc, char **argv, const char **envp)
{
	t_data	data;
	
	argc = 0;
	
	global_init(&data, envp);
	while (1)
	{
		data.rd = readline(PROMPT);
		data.input = strdup(data.rd);

		_init(&data);
		//parser problem // solved by line 43
		if (parser(&data) == ERROR)
			continue ;
		if (execute(&data) == ERROR)
			continue ;
		add_history(data.rd);
		free(data.rd);
	}
	return (0);
}
