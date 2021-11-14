/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/14 17:29:27 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	pipe_and_fork(t_data *data)
{
	if (data->piped_cmd->next)
		if (pipe(data->end) == ERROR)
			return (EXIT_FAILURE);
	data->process = malloc(sizeof(t_process));
	data->process->id = fork();
	if (data->process->id == ERROR)
		return (EXIT_FAILURE);
	g_shell.parent = FALSE;
	if (data->process->id)
		ft_lstadd_back(&data->lst_child_id, ft_lstnew(data->process));
	return (0);
}

static void	daughter_process(t_data *data, int read_end)
{
	if (stream_source(data, read_end, FALSE) || execute_builtin(data))
		exit(EXIT_FAILURE);
	if (is_builtin(*data))
		exit(EXIT_SUCCESS);
	if (!file_search_using_path_var(data))
	{
		if (!is_relative_path(*data) && !data->err_path_env)
			exit(error_msg(*data, M_UNFCMD, 127, NULL));
		data->executable = data->prototype[0];
	}
	if (execve(data->executable, data->prototype,
			(char *const *)env_array(data)))
		execve_errs(*data);
}

static void	piped_commands(t_data *data)
{
	t_list	*tmp;
	int		read_end;

	read_end = -1;
	tmp = data->piped_cmd;
	while (data->piped_cmd)
	{
		if (!data->prototype)
			scan_prototype(data);
		if (pipe_and_fork(data))
			error_msg(*data, NULL, 1, NULL);
		if (data->process->id == 0)
			daughter_process(data, read_end);
		read_end = data->end[0];
		close(data->end[1]);
		data->piped_cmd = data->piped_cmd->next;
		free_2d(data->prototype);
		data->prototype = NULL;
	}
	data->piped_cmd = tmp;
}

static int	simple_builtin_command(t_data *data)
{
	int		ret;
	int		i;

	ret = EXIT_SUCCESS;
	if (stream_source(data, 0, TRUE) || execute_builtin(data))
		ret = EXIT_FAILURE;
	i = -1;
	while (++i < 2)
		if (data->fd[i] != ERROR)
			if (dup2(data->fd[i + 2], i) == ERROR)
				return (error_msg(*data, NULL, 1, NULL));
	close_fds(data);
	free_2d(data->prototype);
	return (ret);
}

int	execute(t_data *data)
{
	if (data->piped_cmd)
	{
		scan_prototype(data);
		if (!data->piped_cmd->next && is_builtin(*data))
		{
			g_shell.exit_status = simple_builtin_command(data);
			return (g_shell.exit_status);
		}
		piped_commands(data);
		close_fds_and_wait(data);
	}
	return (g_shell.exit_status);
}
