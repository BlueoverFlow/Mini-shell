/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/02 13:11:43 by ael-mezz         ###   ########.fr       */
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
	if (execve(data->executable, data->prototype,
			(char *const *)env_array(data)))
		execve_errs(data);
}

static int	pipe_and_fork(t_data *data)
{
	if (data->piped_cmd->next)
	{
		if (pipe(data->end) == ERROR)
			return (EXIT_FAILURE);
	}
	data->process = malloc(sizeof(t_process));
	data->process->id = fork();
	if (data->process->id == ERROR)
		return (EXIT_FAILURE);
	if (data->process->id)
		ft_lstadd_back(&data->lst_child_id, ft_lstnew(data->process));
	return (EXIT_SUCCESS);
}

static int	simple_command(t_data *data)
{
	int	ret;

	ret = FALSE;
	if (!data->piped_cmd->next)
	{
		scan_command(data);
		if (stream_source(data, 0, TRUE) || builtin(data))
			ret = EXIT_FAILURE;
		else if (data->is_builtin == TRUE)
			ret = TRUE;
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
		free_2d(data->prototype);
	}
	return (ret);
}

static void	piped_commands(t_data *data)
{
	t_list	*tmp;
	int		read_end;

	read_end = -1;
	tmp = data->piped_cmd;
	while (data->piped_cmd)
	{
		scan_command(data);
		if (pipe_and_fork(data))
			error_msg(data, M_ARGERR, NULL);
		if (data->process->id == 0)
			daughter_process(data, read_end);
		read_end = data->end[0];
		close(data->end[1]);
		data->piped_cmd = data->piped_cmd->next;
		free_2d(data->prototype);
	}
	data->piped_cmd = tmp;
}

int	execute(t_data *data)
{
	int	ret;

	ret = simple_command(data);
	if (ret)
	{
		if (ret == EXIT_FAILURE)
			data->exit_status = ret;
		else
			ret = EXIT_SUCCESS;
		return (ret);
	}
	piped_commands(data);
	close_fds_and_wait(data);
	return (data->exit_status);
}
