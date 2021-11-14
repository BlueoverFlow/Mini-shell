/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:22:16 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/14 17:28:58 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	execute_edited_prototype(t_data *data, char	*cmd)
{
	char		**tmp;

	tmp = data->prototype;
	data->prototype = ft_split(cmd, ' ');
	free(cmd);
	execute_builtin(data);
	free_2d(data->prototype);
	data->prototype = tmp;
}

BOOL	is_relative_path(t_data data)
{
	return (data.prototype[0][0] == '~' || data.prototype[0][0] == '.'
		|| data.prototype[0][0] == '/');
}

int	error_msg(t_data data, char *message, int exit_code, char *file)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (data.prototype)
	{
		ft_putstr_fd(data.prototype[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (file)
	{
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd("': ", STDERR_FILENO);
	}
	if (message)
		ft_putstr_fd(message, STDERR_FILENO);
	else
		perror(NULL);
	return (exit_code);
}

void	execve_errs(t_data data)
{
	if ((errno == ENOENT || errno == EFAULT)
		&& (is_relative_path(data) || data.err_path_env))
	{
		errno = ENOENT;
		exit(error_msg(data, NULL, 127, NULL));
	}
	else if (errno == ENOENT || errno == EFAULT)
		exit(error_msg(data, M_UNFCMD, 127, NULL));
	else if (errno == EACCES)
	{
		errno = EPERM;
		exit(error_msg(data, NULL, 126, NULL));
	}
}
