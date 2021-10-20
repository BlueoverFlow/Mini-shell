/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 18:48:21 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int daughter_process(t_data *data, int read_end)
{
	if (stream_source(data, read_end) == ERROR
		|| builtin(data) == ERROR)
	{
		perror("minishell: ");
		exit(EXIT_FAILURE);
	}
	builtin(data);
	if (!data->is_builtin)
	{
		if (file_search_using_path_var(data) == ERROR)
			exit (EXIT_FAILURE);
		if (execve(data->executable, data->prototype, NULL))
			assign_exit_status(data);
	}
	exit(EXIT_FAILURE);
}

static int	pipe_and_fork(t_data *data)
{
	if (data->piped_cmd->next)
	{
		if (pipe(data->end) == -1)
			return(ERROR);
	}
	data->id = fork();
	if (data->id == ERROR)
		return(ERROR);
	return (1);
}

static int simple_command(t_data *data)
{
	int ret;

	ret = 0;
	data->simple_cmd = FALSE;
	if (!data->piped_cmd->next)
	{
		data->simple_cmd = TRUE;
		if (stream_source(data, 0) == ERROR
			|| builtin(data) == ERROR)
		{
			data->exit_status = 1;
			ret = ERROR;
		}
		if (data->is_builtin)
		{
			free_2d(data->prototype);
			ret = 1;
		}
		if (data->fd[0] != ERROR)
			dup2(data->fd[2], STDIN_FILENO);
		if (data->fd[1] != ERROR)
			dup2(data->fd[3], STDOUT_FILENO);
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
		return (ret);
	while (data->piped_cmd)
	{
		if (pipe_and_fork(data) == ERROR)
			return (ERROR);
		if (data->id == 0)
			daughter_process(data, read_end);
		read_end = data->end[0];
		close(data->end[1]);
		data->piped_cmd = data->piped_cmd->next;
	}
	close_fds_and_wait(data);
	return (1);
}
