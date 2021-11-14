/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 17:20:15 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/14 17:26:30 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	find_char(char *str, char c)
{
	int	i;

	i = 0;
	if (str)
	{
		while (str[i])
		{
			if (str[i] == c)
				return (i);
			i++;
		}
	}
	return (-1);
}

int	syntax_checking(t_data *data, int i)
{
	int			l;
	t_list		*last;

	l = ft_strlen(data->input) - 1;
	if (data->command->file)
	{
		last = ft_lstlast(data->command->file);
		data->file_data = last->content;
	}
	if ((data->file_data && data->file_data->id != ERROR
			&& !data->file_data->path)
		|| (data->input[l] == '|')
		|| (data->quoting_state != UNQUOTED
			&& !data->input[i + 1] && !data->passive))
		return (EXIT_FAILURE);
	data->command = NULL;
	return (EXIT_SUCCESS);
}
