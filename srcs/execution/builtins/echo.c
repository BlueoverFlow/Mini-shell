/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/15 18:57:17 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static int	n_option(t_data data, int *i)
{
	size_t	j;
	int		new_line;

	*i = 0;
	new_line = 1;
	while (data.prototype[++(*i)])
	{
		j = 0;
		if (data.prototype[*i][j++] == '-' && data.prototype[*i][j]
			&& data.prototype[*i][j] == 'n')
		{
			while (data.prototype[*i][j] == 'n')
				j++;
			if (data.prototype[*i][j] && data.prototype[*i][j] != 'n')
				return (new_line);
			new_line = 0;
		}
		else
			return (new_line);
	}
	return (new_line);
}

int	echo(t_data data)
{
	int		i;
	int		n;

	if (!data.prototype[1])
	{
		printf("\n");
		return (0);
	}
	n = n_option(data, &i);
	while (data.prototype[i])
	{
		ft_putstr_fd(data.prototype[i++], STDOUT_FILENO);
		if (data.prototype[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (n)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
