/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/21 12:24:04 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	daughter_process(t_data *data, int read_end)
{
	if (stream_source(data, read_end, FALSE) || builtin(data))
		exit(EXIT_FAILURE);
	if (data->is_builtin == TRUE)
		exit(EXIT_SUCCESS);
	if (!file_search_using_path_var(data))
		data->executable = data->prototype[0];
	if (execve(data->executable, data->prototype, NULL))
		execve_errs(data);
}

static int	pipe_and_fork(t_data *data)
{
	if (data->piped_cmd->next)
	{
		if (pipe(data->end) == ERROR)
			return(EXIT_FAILURE);
	}
	data->id = fork();
	if (data->id == ERROR)
		return(EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int simple_command(t_data *data)
{
	int	ret;

	ret = FALSE;
	if (!data->piped_cmd->next)
	{
		if (stream_source(data, 0, TRUE) || builtin(data))
			ret = EXIT_FAILURE;
		else if (data->is_builtin == TRUE)
		{
			free_2d(data->prototype);
			ret = TRUE;
		}
		if (data->fd[0] != ERROR)
		{
			if (dup2(data->fd[2], STDIN_FILENO) == ERROR)
				return (error_msg(data, M_ARGERR, NULL));
		}
		if (data->fd[1] != ERROR)
		{
			if (dup2(data->fd[3], STDOUT_FILENO) == ERROR)
				return (error_msg(data, M_ARGERR, NULL));
		}
		close_fds(data);
	}
	return (ret);
}

int	execute(t_data *data)
{
	int		read_end;
	int		ret;

	read_end = -1;
	scan_command(data);
	ret = simple_command(data);
	if (ret)
	{
		if (ret == EXIT_FAILURE)
			data->exit_status = ret;
		else
			ret = EXIT_SUCCESS;
		return (ret);
	}
	while (data->piped_cmd)
	{
		if (pipe_and_fork(data))
			error_msg(data, M_ARGERR, NULL);
		if (data->id == 0)
			daughter_process(data, read_end);
		read_end = data->end[0];
		close(data->end[1]);
		data->piped_cmd = data->piped_cmd->next;
	}
	close_fds_and_wait(data);
	return (EXIT_SUCCESS);
}
