/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 14:29:22 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static size_t	n_option(t_data data)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (data.prototype[++i])
	{
		j = 0;
		if (data.prototype[i][j++] == '-' && data.prototype[i][j]
			&& data.prototype[i][j] == 'n')
		{
			while (data.prototype[i][j] == 'n')
				j++;
			if (data.prototype[i][j] && data.prototype[i][j] != 'n')
				return (1);
		}
		else
			return (i);
	}
	return (i);
}

int	echo(t_data data)
{
	size_t	i;
	int		n;

	n = 1;
	if (!data.prototype[1])
	{
		printf("\n");
		return (0);
	}
	i = n_option(data);
	if (i > 1)
		n = 0;
	else
		n = 1;
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
