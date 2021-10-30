/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 14:22:16 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/30 18:23:10 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	close_fds(t_data *data)
{
	int	i;

	i = -1;
	while (++i < 4)
		close(data->fd[i]);
	close(data->end[0]);
	close(data->end[1]);
}

void	close_fds_and_wait(t_data *data)
{
	int		stat;

	close_fds(data);
	while (data->lst_child_id)
	{
		data->process = data->lst_child_id->content;
		waitpid(data->process->id, &stat, 0);
		data->lst_child_id = data->lst_child_id->next;
	}
	if (WIFEXITED(stat))
		data->exit_status = WEXITSTATUS(stat);
}

void	error_prompt(t_data *data, char *arg)
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
