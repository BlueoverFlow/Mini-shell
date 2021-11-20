/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/19 15:42:56 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

/*
pathetic, we arent allowed to use arrays
*/

BOOL	is_builtin(t_data data)
{
	char	**builtins;
	int		i;

	builtins = ft_split("echo export env pwd unset exit cd CD cD Dc Cd", ' ');
	i = -1;
	while (builtins[++i])
	{
		if (!ft_strcmp(data.prototype[0], builtins[i]))
		{
			free_2d(builtins);
			return (TRUE);
		}
	}
	free_2d(builtins);
	return (FALSE);
}

int	execute_builtin(t_data *data)
{
	if (!ft_strcmp(data->prototype[0], "echo"))
		return (echo(*data));
	else if (!ft_strcmp(data->prototype[0], "export"))
		return (export(data));
	else if (!ft_strcmp(data->prototype[0], "env"))
		return (env(*data));
	else if (!ft_strcmp(data->prototype[0], "pwd"))
		return (pwd(*data));
	else if (!ft_strcmp(data->prototype[0], "unset"))
		return (unset(data));
	else if (!ft_strcmp(data->prototype[0], "exit"))
		return (exit_shell(*data));
	else if (is_builtin(*data))
		return (cd(data));
	return (EXIT_SUCCESS);
}
