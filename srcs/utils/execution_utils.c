/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:22:16 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 14:29:36 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	execute_builtin(t_data *data, char	*cmd)
{
	char		**tmp;

	tmp = data->prototype;
	data->prototype = ft_split(cmd, ' ');
	free(cmd);
	builtin(data);
	free_2d(data->prototype);
	data->prototype = tmp;
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
		&& (data.prototype[0][0] == '~' || data.prototype[0][0] == '.'
		|| data.prototype[0][0] == '/' || data.err_path_env))
		exit(error_msg(data, strerror(ENOENT), 127, NULL));
	else if (errno == ENOENT || errno == EFAULT)
		exit(error_msg(data, M_UNFCMD, 127, NULL));
	else if (errno == EACCES)
		exit(error_msg(data, strerror(EPERM), 126, NULL));
}
