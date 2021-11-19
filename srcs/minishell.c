/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 08:15:00 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/19 14:11:12 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	global_init(t_data *data, int argc,
	char **argv, char *const	*envp)
{
	data->exported = NULL;
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	build_env_vars(data, envp);
	g_shell.exit_status = 0;
	data->is_heredoc = FALSE;
}

static void	_init(t_data *data)
{
	g_shell.parent = TRUE;
	data->piped_cmd = NULL;
	data->word = NULL;
	data->lst_child_id = NULL;
	data->quoting_state = UNQUOTED;
	data->passive = FALSE;
	data->prototype = NULL;
	data->command = NULL;
	data->file_data = NULL;
	data->end[1] = ERROR;
	data->end[0] = ERROR;
	data->fd[0] = ERROR;
	data->fd[1] = ERROR;
	data->fd[2] = ERROR;
	data->fd[3] = ERROR;
}

static void	free_leaks(t_data data)
{
	t_list	*tmp;
	t_list	*tmp_2;

	tmp = data.piped_cmd;
	while (data.piped_cmd)
	{
		data.command = data.piped_cmd->content;
		tmp_2 = data.command->file;
		while (tmp_2)
		{
			data.file_data = tmp_2->content;
			free(data.file_data->path);
			free(data.file_data->path_2);
			tmp_2 = tmp_2->next;
		}
		free_list(&data.command->prototype);
		free_list(&data.command->file);
		data.piped_cmd = data.piped_cmd->next;
	}
	data.piped_cmd = tmp;
	free_list(&data.piped_cmd);
}

int	main(int argc, char **argv, char *const envp[])
{
	t_data	data;

	global_init(&data, argc, argv, envp);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	while (1)
	{
		_init(&data);
		data.input = readline(PROMPT);
		if (!data.input || !*data.input || parser(&data) || execute(&data))
			;
		else
			g_shell.exit_status = 0;
		free_leaks(data);
		if (!data.input)
			execute_edited_prototype(&data, ft_strdup("exit"));
		else if (*data.input)
			add_history(data.input);
		free(data.input);
	}
	return (0);
}
