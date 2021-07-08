/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/07/06 11:07:37 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(t_data *data, char **prototype)
{
	if (ft_strcmp(prototype[0], "$?"))
		data->exit_status = 0;
	if (!ft_strcmp(prototype[0], "echo"))
		echo(prototype);
	else if (!ft_strcmp(prototype[0], "export"))
		export(data, prototype);
	else if (!ft_strcmp(prototype[0], "env"))
		env(data, prototype);
	else if (!ft_strcmp(prototype[0], "$?"))
		out(data, ": command not found\n", 11);
	// return ((data->exit_status != 0 ) ? ERROR : 1);
	if (data->exit_status != 0)
		return (ERROR);
	else
		return (1);
}
