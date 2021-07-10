/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/07/10 09:52:42 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(t_data *data, char **prototype)
{
	int in;

	in = 0;
	if (ft_strcmp(prototype[0], "$?"))
		data->exit_status = 0;
	if (!ft_strcmp(prototype[0], "echo"))
	{
		in = 1;
		echo(prototype);
	}
	else if (!ft_strcmp(prototype[0], "export"))
	{
		in = 1;
		export(data, prototype);
	}
	else if (!ft_strcmp(prototype[0], "env"))
	{
		in = 1;
		env(data, prototype);
	}
	else if (!ft_strcmp(prototype[0], "$?"))
	{
		in = 1;
		out(data, ": command not found\n", 11);
	}
	// return ((data->exit_status != 0 ) ? ERROR : 1);
	if (data->exit_status != 0)
		return (ERROR);
	else if (in)
		return (1);
	else
		return 0;
}
