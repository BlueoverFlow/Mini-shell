/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:48:47 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 18:05:22 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	initiate_vars(t_data *data, int file_number, t_h_d *h_d)
{
	int	ret;

	h_d->input = NULL;
	h_d->file_name = ft_strjoin_and_free
		("/tmp/.heredoc_", ft_itoa(file_number), 2);
	h_d->fd = open(h_d->file_name, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (h_d->fd < 3)
	{
		ret = error_msg(*data, M_ARGERR, 1, h_d->file_name);
		free(h_d->file_name);
		return (ret);
	}
	return (0);
}

static int	input_stream_literal(t_data *data)
{
	static int	file_number = 0;
	t_h_d		h_d;

	file_number++;
	if (initiate_vars(data, file_number, &h_d))
		return (EXIT_FAILURE);
	g_shell.heredoc = TRUE;
	while (1)
	{
		h_d.input = readline("> ");
		if (!h_d.input || !g_shell.heredoc
			|| !ft_strcmp(data->file_data->path, h_d.input))
			break ;
		write(h_d.fd, h_d.input, ft_strlen(h_d.input));
		write(h_d.fd, "\n", 1);
		free(h_d.input);
	}
	g_shell.heredoc = FALSE;
	free(h_d.input);
	data->file_data->path_2 = h_d.file_name;
	close(h_d.fd);
	return (0);
}

int	hundle_heredoc(t_data *data)
{
	t_list		*tmp;
	t_list		*tmp_2;

	tmp = data->piped_cmd;
	while (data->piped_cmd)
	{
		data->command = data->piped_cmd->content;
		tmp_2 = data->command->file;
		while (data->command->file)
		{
			data->file_data = data->command->file->content;
			if (data->file_data->id == HEREDOC)
				if (input_stream_literal(data))
					return (EXIT_FAILURE);
			data->command->file = data->command->file->next;
		}
		data->command->file = tmp_2;
		data->piped_cmd = data->piped_cmd->next;
	}
	data->piped_cmd = tmp;
	return (EXIT_SUCCESS);
}
