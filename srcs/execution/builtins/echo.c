/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/21 08:05:41 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static size_t	n_option(t_data *data)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (data->prototype[++i])
	{
		j = 0;
		if (data->prototype[i][j++] == '-'
			&& data->prototype[i][j] && data->prototype[i][j] == 'n')
		{
			while (data->prototype[i][j] == 'n')
				j++;
			if (data->prototype[i][j] && data->prototype[i][j] != 'n')
				return (1);
		}
		else
			return (i);
	}
	return (i);
}

int	echo(t_data *data)
{
	size_t	i;
	int		n;

	n = 1;
	if (!data->prototype[1])
	{
		printf("\n");
		return (0);
	}
	i = n_option(data);
	if (i > 1)
		n = 0;
	else
		n = 1;
	while (data->prototype[i])
	{
		printf("%s", data->prototype[i++]);
		if (data->prototype[i])
			printf(" ");
	}
	if (n)
		printf("\n");
	return (EXIT_SUCCESS);
}
