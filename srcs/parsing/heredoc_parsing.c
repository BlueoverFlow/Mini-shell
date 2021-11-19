/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:48:47 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/19 14:09:52 by ael-mezz         ###   ########.fr       */
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

static void	catch_input(t_data data, t_h_d h_d)
{
	while (1)
	{
		h_d.input = readline("> ");
		if (!h_d.input || !ft_strcmp(data.file_data->path, h_d.input))
		{
			if (h_d.input)
				free(h_d.input);
			break ;
		}
		h_d.input = expand_env_vars(&data, h_d.input);
		write(h_d.fd, h_d.input, ft_strlen(h_d.input));
		write(h_d.fd, "\n", 1);
		free(h_d.input);
	}
}

static int	input_stream_literal(t_data *data)
{
	static int	file_number = 0;
	t_h_d		h_d;

	file_number++;
	if (initiate_vars(data, file_number, &h_d))
		return (ERROR);
	data->is_heredoc = TRUE;
	catch_input(*data, h_d);
	data->is_heredoc = FALSE;
	data->file_data->path_2 = h_d.file_name;
	close(h_d.fd);
	return (0);
}

int	restore_adrs(t_data *data, t_list *tmp, t_list *tmp_2)
{
	data->command->file = tmp_2;
	data->piped_cmd = tmp;
	return (EXIT_FAILURE);
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
					return (restore_adrs(data, tmp, tmp_2));
			data->command->file = data->command->file->next;
		}
		data->command->file = tmp_2;
		data->piped_cmd = data->piped_cmd->next;
	}
	data->piped_cmd = tmp;
	return (EXIT_SUCCESS);
}
