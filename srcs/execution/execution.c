/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 10:06:45 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 09:41:27 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int give_input(t_data *data)
{
	t_list *tmp;
	int		fd;

	fd = NO_FILE;
	if (data->outfile)
	{
		tmp = data->command->file;
		while (tmp && data->outfile)
		{
			data->file_data = tmp->content;
			if (data->file_data->id == REDIRECTED_OUTPUT)
			{
				if ((fd = open(data->file_data->path, O_CREAT | O_WRONLY | O_TRUNC, 0666)) < 3)
					return (ERROR);
				if (data->outfile-- > 1)
					close(fd);
			}
			else if (data->file_data->id == APPENDED_REDIRECTED_OUTPUT)
			{
				if ((fd = open(data->file_data->path, O_CREAT | O_WRONLY | O_APPEND, 666)) < 3)
					return (ERROR);
				if (data->outfile-- > 1)
					close(fd);
			}
		}
	}
	return (fd);
}

static int take_ouput(t_data *data)
{
	t_list *tmp;
	int		fd; 

	fd = NO_FILE;
	if (data->infile)
	{
		tmp = data->command->file;
		while (tmp && data->infile)
		{
			data->file_data = tmp->content;
			if (data->file_data->id == REDIRECTED_INPUT)
			{
				if ((fd = open(data->file_data->path, O_RDONLY)) < 3)
					return (ERROR);
				if (data->infile-- > 1)
					close(fd);
			}
			else if (data->file_data->id == HEREDOC)
			{
				if ((fd = open(data->document, O_RDONLY)) < 3)
					return (ERROR);
				if (data->infile-- > 1)
					close(fd);
			}
		}
	}
	return (fd);
}

static int check_the_ends(t_data *data)
{
	t_list *tmp;

	data->infile = FALSE;
	data->outfile = FALSE;
	if (data->command->file)
	{
		tmp = data->command->file;
		while (tmp)
		{
			data->file_data = tmp->content;
			if (data->file_data->id == HEREDOC || data->file_data->id == REDIRECTED_INPUT)
				data->infile++;
			if (data->file_data->id == APPENDED_REDIRECTED_OUTPUT || data->file_data->id == REDIRECTED_OUTPUT)
				data->outfile++;
			tmp = tmp->next;
		}
	}
	return (1);
}

static int from_fd_to_stdin(t_data *data, int read_end, int fd_in)
{
	if (fd_in == -2)
		fd_in = read_end;
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		return(error_msg(data, NULL, PERROR));
	}
	close(fd_in);
	return (1);
}

static int from_fd_to_stdout(t_data *data, int fd_out)
{
	if (fd_out == -2)
		fd_out = data->end[1];
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		return(error_msg(data, NULL, PERROR));
	}
	close(fd_out);
	return (1);
}

static int	duplicate(t_data *data, int fd_in, int fd_out, int read_end)
{
	if (fd_in == ERROR || fd_out == ERROR)
		return (ERROR);
	if (data->command->prototype && (data->piped_cmd->previous || fd_in != NO_FILE))
	{
		if (from_fd_to_stdin(data, read_end, fd_in) == ERROR)
			return (ERROR);
	}
	if (data->command->prototype && (data->piped_cmd->next || fd_out != NO_FILE))
	{
		if (from_fd_to_stdout(data, fd_out) == ERROR)
			return (ERROR);
	}
	return (1);
}

static int call_the_daughter(t_data *data, char **prototype, int read_end)
{
	check_the_ends(data);
	if (duplicate(data, take_ouput(data), give_input(data), read_end) == ERROR)
	{
		perror("minishell: ");
		exit(EXIT_FAILURE);
	}
	builtin(data, prototype);
	if (file_search_using_path_var(data, prototype[0]) == ERROR)
		exit (EXIT_FAILURE);
	if (!data->is_builtin && execve(data->executable, prototype, NULL))
		perror("minishell: ");
	exit(EXIT_FAILURE);
}

static int	pipe_and_fork(t_data *data)
{
	if (data->piped_cmd->next)
	{
		if (pipe(data->end) == -1)
			return(error_msg(data, NULL, PERROR));
	}
	if ((data->id = fork()) == ERROR)
		return(error_msg(data, NULL, PERROR));
	return (1);
}

static void close_fds_and_wait(t_data *data)
{
	int		stat;

	close(data->end[0]);
	close(data->end[1]);
	waitpid(-1, &stat, 0);
	waitpid(data->id, &stat, 0);
	if (WIFEXITED(stat))
		data->exit_status = WEXITSTATUS(stat);
}

static int simple_command(t_data *data, char **prototype)
{
	if (!data->piped_cmd->next)
	{
		builtin(data, prototype);
		if (data->is_builtin)
		{
			free_2d(prototype);
			return (1);
		}
	}
	return (0);
}

int	execute(t_data *data)
{
	char	**prototype;
	int		read_end;
	int		ret;

	read_end = -1;
	prototype = scan_command(data);
	ret = simple_command(data, prototype);
	if (ret)
		return (ret);
	while (data->piped_cmd)
	{
		if (pipe_and_fork(data) == ERROR)
			return (ERROR);
		read_end = data->end[0];
		if (data->id == 0)
			call_the_daughter(data, prototype, read_end);
		close(data->end[1]);
		data->piped_cmd = data->piped_cmd->next;
	}
	close_fds_and_wait(data);
	return (1);
}
