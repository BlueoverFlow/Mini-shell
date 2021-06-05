/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/05 10:40:42 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void _init(t_data *data)
{
	data->field_status = NULL;
	data->garbage = NULL;
	data->tokens = NULL;
	data->lines = NULL;
	data->is_one_token = FALSE;
	data->is_quoted = FALSE;
	data->is_separated = FALSE;
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
	}
	return (0);
}