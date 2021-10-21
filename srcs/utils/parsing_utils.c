/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 17:20:15 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/21 10:59:12 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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
	if ((data->file_data && data->file_data->id != -1 && !data->file_data->path)
		|| (data->input[l] == '|')
		|| (data->quoting_state != UNQUOTED
			&& !data->input[i + 1] && !data->passive))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
