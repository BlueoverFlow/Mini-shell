/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/18 17:30:45 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void _init(t_data *data)
{
	data->commands = NULL;
	data->garbage = NULL;
	data->tokens = NULL;
	data->lines = NULL;
	data->is_one_token = FALSE;
	data->is_quoted = FALSE;
}

int main()
{
	t_data data;

	while (READ)
	{
		_init(&data);
		ft_lstadd_back(&data.garbage, ft_lstnew(data.input));
		parser(&data, 0);
		free_list(&data.tokens);
		free_list(&data.garbage);
		while (1)
		;
	}
	return (0);
}