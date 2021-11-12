/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 17:45:42 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static BOOL	is_cd_cmd(char *cmd)
{
	if (ft_strlen(cmd) == 2 && (cmd[0] == 'c' || cmd[0] == 'C')
		&& (cmd[1] == 'd' || cmd[1] == 'D'))
		return (TRUE);
	return (FALSE);
}

int	builtin(t_data *data)
{
	data->is_builtin = TRUE;
	if (!ft_strcmp(data->prototype[0], "echo"))
		return (echo(*data));
	else if (!ft_strcmp(data->prototype[0], "export"))
		return (export(data));
	else if (!ft_strcmp(data->prototype[0], "env"))
		return (env(*data));
	else if (is_cd_cmd(data->prototype[0]))
		return (cd(data));
	else if (!ft_strcmp(data->prototype[0], "pwd"))
		return (pwd(*data));
	else if (!ft_strcmp(data->prototype[0], "unset"))
		return (unset(data));
	else if (!ft_strcmp(data->prototype[0], "exit"))
		return (exit_shell(*data));
	data->is_builtin = FALSE;
	return (EXIT_SUCCESS);
}
