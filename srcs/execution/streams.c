/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   streams.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 11:00:30 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/23 08:38:32 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	scan_files_list(t_data *data)
{
	t_list	*tmp;

	data->infile = FALSE;
	data->outfile = FALSE;
	tmp = data->command->file;
	while (tmp)
	{
		data->file_data = tmp->content;
		if (data->file_data->id == HEREDOC
			|| data->file_data->id == REDIRECTED_INPUT)
			data->infile++;
		else if (data->file_data->id == APPENDED_REDIRECTED_OUTPUT
			|| data->file_data->id == REDIRECTED_OUTPUT)
			data->outfile++;
		tmp = tmp->next;
	}
}

static int	open_file(t_data *data, int *stream_type)
{
	int		flags;
	char	*path;
	int		fd;

	*stream_type = FALSE;
	path = data->file_data->path;
	if (data->file_data->id == HEREDOC
		|| data->file_data->id == REDIRECTED_INPUT)
	{
		flags = O_RDONLY;
		*stream_type = TRUE;
		if (data->file_data->id == HEREDOC)
			path = data->file_data->path_2;
	}
	else if (data->file_data->id == REDIRECTED_OUTPUT)
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	else
		flags = O_CREAT | O_WRONLY | O_APPEND;
	fd = open(path, flags, S_IRWXU);
	if (fd < 3)
		fd = error_msg(data, M_ARGERR, path);
	return (fd);
}

static void	close_unnecessary_fds(t_data *data, int file, int stream_type)
{
	if (stream_type)
	{
		if (--data->infile)
			close(file);
		data->fd[0] = file;
	}
	else
	{
		if (--data->outfile)
			close(file);
		data->fd[1] = file;
	}
}

static int	cmd_input_output(t_data *data)
{
	t_list	*tmp;
	int		file;
	int		stream_type;

	tmp = data->command->file;
	while (tmp)
	{
		data->file_data = tmp->content;
		file = open_file(data, &stream_type);
		if (file < 3)
			return (file);
		close_unnecessary_fds(data, file, stream_type);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

int	stream_source(t_data *data, int read_end, BOOL	simple_cmd)
{
	scan_files_list(data);
	data->fd[2] = dup(STDIN_FILENO);
	data->fd[3] = dup(STDOUT_FILENO);
	if (cmd_input_output(data) || data->fd[2] == ERROR
		|| data->fd[3] == ERROR)
		return (EXIT_FAILURE);
	if (data->piped_cmd
		&& (data->piped_cmd->previous || data->fd[0] != ERROR))
	{
		if (data->fd[0] == ERROR)
			data->fd[0] = read_end;
		if (dup2(data->fd[0], STDIN_FILENO) == ERROR)
			return (EXIT_FAILURE);
	}
	if (data->piped_cmd
		&& (data->piped_cmd->next || data->fd[1] != ERROR))
	{
		if (data->fd[1] == ERROR)
			data->fd[1] = data->end[1];
		if (dup2(data->fd[1], STDOUT_FILENO) == ERROR)
			return (ERROR);
	}
	if (simple_cmd == FALSE)
		close_fds(data);
	return (EXIT_SUCCESS);
}
