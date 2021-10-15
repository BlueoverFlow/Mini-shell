/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/15 10:05:21 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	builtin(t_data *data, char **prototype)
{
	data->is_builtin = TRUE;
	data->is_env = FALSE;
	if (!ft_strcmp(prototype[0], "echo"))
		echo(data, prototype);
	else if (!ft_strcmp(prototype[0], "export"))
		export(data, prototype);
	else if (!ft_strcmp(prototype[0], "env"))
		env(data, prototype);
	else if (!ft_strcmp(prototype[0], "cd"))
		cd(data, prototype[1]);
	else if (!ft_strcmp(prototype[0], "unset"))
		unset(data, prototype);
	else
		data->is_builtin = FALSE;
	return ((data->exit_status != 0 ) ? ERROR : 1);
}
