/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 09:37:58 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

void	builtin(t_data *data, char **prototype)
{
	data->is_builtin = TRUE;
	data->is_env = FALSE;
	if (!ft_strcmp(prototype[0], "echo"))
		echo(prototype);
	else if (!ft_strcmp(prototype[0], "export"))
		export(data, prototype);
	else if (!ft_strcmp(prototype[0], "env"))
		env(data, prototype);
	else if (!ft_strcmp(prototype[0], "cd"))
		cd(prototype[1]);
	else if (!ft_strcmp(prototype[0], "unset"))
		unset(data, prototype);
	else
		data->is_builtin = FALSE;
}
