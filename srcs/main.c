/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/07 18:05:50 by ael-mezz         ###   ########.fr       */
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
	data->unset_cmd = FALSE;
}

int	main(int argc, char **argv, const char **envp)
{
	t_data	data;
	
	global_init(&data, envp);
	while (1)
	{
		data.input = readline(PROMPT);
		_init(&data);
		if (!data.input || !*data.input	|| parser(&data) == ERROR || execute(&data) == ERROR)
			;
		if (data.input && *data.input)
		{
			add_history(data.input);
			free(data.input);
		}
		else if (!data.input)
			printf("\n");
	}
	return (0);
}
