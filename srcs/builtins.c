/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/10 13:21:27 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin(t_data *data, char **prototype)
{
	data->is_builtin = TRUE;
	if (!ft_strcmp(prototype[0], "echo"))
		echo(data, prototype);
	else if (!ft_strcmp(prototype[0], "export"))
		export(data, prototype);
	else if (!ft_strcmp(prototype[0], "env"))
		env(data, prototype);
	else if (!ft_strcmp(prototype[0], "cd"))
		cd(data, prototype[1]);
	else if (!ft_strcmp(prototype[0], "CD") || !ft_strcmp(prototype[0], "Cd") || !ft_strcmp(prototype[0], "cD"))
		return 0 ;
	else if (!ft_strcmp(prototype[0], "unset"))
		unset(data, prototype);
	else
		data->is_builtin = FALSE;
	return ((data->exit_status != 0 ) ? ERROR : 1);
}
