/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:22:16 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/10 17:46:11 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	error_prompt(t_data *data, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (data->prototype)
	{
		ft_putstr_fd(data->prototype[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg)
	{
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': ", STDERR_FILENO);
	}
}

static int	error_msg_2(t_data *data, int errno_code, char *file)
{
	ft_putstr_fd("syntax error!\n", STDERR_FILENO);
	return (258);
}

int	error_msg(t_data *data, int errno_code, char *file)
{
	error_prompt(data, file);
	if (errno_code == M_NOCMD || errno_code == M_NOEXENT)
	{
		if (errno_code == M_NOCMD)
			ft_putstr_fd("command not found\n", STDERR_FILENO);
		else
			ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		return (127);
	}
	else if (errno_code == M_ARGERR || errno_code == M_NOVALID)
	{
		if (errno_code == M_NOVALID)
			ft_putstr_fd("not a valid identifier\n", STDERR_FILENO);
		else if (errno_code == M_HMERR)
			ft_putstr_fd("HOME not set\n", STDERR_FILENO);
		else
			perror(NULL);
		return (1);
	}
	else if (errno_code == M_BADACCES)
	{
		ft_putstr_fd("can't access/execute\n", STDERR_FILENO);
		return (126);
	}
	return (error_msg_2(data, errno_code, file));
}

void	execve_errs(t_data *data)
{
	if ((errno == ENOENT || errno == EFAULT)
		&& (data->prototype[0][0] == '~' || data->prototype[0][0] == '.'
		|| data->prototype[0][0] == '/' || data->err_path_env))
		exit(error_msg(data, M_NOEXENT, NULL));
	else if (errno == ENOENT || errno == EFAULT)
		exit(error_msg(data, M_NOCMD, NULL));
	else if (errno == EACCES)
		exit(error_msg(data, M_BADACCES, NULL));
}
