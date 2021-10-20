/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 14:25:36 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static size_t	n_option(char **prototype)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (prototype[++i])
	{
		j = 0;
		if (prototype[i][j++] == '-' && prototype[i][j] && prototype[i][j] == 'n')
		{
			while (prototype[i][j] == 'n')
				j++;
			if (prototype[i][j] && prototype[i][j] != 'n')
				return (1);
		}
		else
			return (i);
	}
	return (i);
}

int	echo(char **prototype)
{
	size_t	i;
	int		n;

	n = 1;
	if (!prototype[1])
	{
		printf("\n");
		return (0);
	}
	i = n_option(prototype);
	if (i > 1)
		n = 0;
	else
		n = 1;
	while (prototype[i])
	{
		printf("%s", prototype[i++]);
		if (prototype[i])
			printf(" ");
	}
	if (n)
		printf("\n");
	return (1);
}
