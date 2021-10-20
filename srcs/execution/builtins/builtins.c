/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 18:43:03 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int		builtin(t_data *data)
{
	data->is_builtin = TRUE;
	if (!ft_strcmp(data->prototype[0], "echo"))
		echo(data);
	else if (!ft_strcmp(data->prototype[0], "export"))
		export(data);
	else if (!ft_strcmp(data->prototype[0], "env"))
		env(data);
	else if (!ft_strcmp(data->prototype[0], "cd"))
		cd(data->prototype[1]);
	else if (!ft_strcmp(data->prototype[0], "unset"))
		unset(data);
	else
		data->is_builtin = FALSE;
	return (1);
}
