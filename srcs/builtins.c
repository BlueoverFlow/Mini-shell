/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/27 10:27:43 by ael-mezz         ###   ########.fr       */
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
	return ((data->exit_status != 0 ) ? ERROR : 1);
}