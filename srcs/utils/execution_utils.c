/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:22:16 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/21 12:16:55 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	is_directory(char *file)
{
	int	fd;

	fd = open(file, O_DIRECTORY);
	if (fd >= 3)
	{
		close (fd);
		errno = EISDIR;
		return (1);
	}
	return (0);
}

void	close_fds(t_data *data)
{
	close(data->end[0]);
	close(data->end[1]);
	close(data->fd[0]);
	close(data->fd[1]);
	close(data->fd[2]);
	close(data->fd[3]);
}

void	close_fds_and_wait(t_data *data)
{
	int		stat;

	close_fds(data);
	waitpid(-1, &stat, 0);
	waitpid(data->id, &stat, 0);
	if (WIFEXITED(stat))
		data->exit_status = WEXITSTATUS(stat);
}

static void error_prompt(t_data *data, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(data->prototype[0], STDERR_FILENO);
		ft_putstr_fd(": `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': ", STDERR_FILENO);
	}
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
		else
			perror(NULL);
	}
	else if (errno_code == M_BADACCES)
	{
		perror(NULL);
		return (126);
	}
	else if (errno_code == M_STXERR)
	{
		ft_putstr_fd("syntax error!\n", STDERR_FILENO);
		return (258);
	}
	return (1);
}

void	execve_errs(t_data *data)
{
	if ((errno == ENOENT || errno == EFAULT)
		&& ((data->prototype[0][0] == '~' || data->prototype[0][0] == '.'
			|| data->prototype[0][0] == '/') || data->err_path_env))
		exit(error_msg(data, M_NOEXENT, data->prototype[0]));
	else if (errno == ENOENT || errno == EFAULT)
		exit(error_msg(data, M_NOCMD, data->prototype[0]));
	else if (is_directory(data->executable) || errno == EACCES)
		exit(error_msg(data, M_BADACCES, data->prototype[0]));
}
