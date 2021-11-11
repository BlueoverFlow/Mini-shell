/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 16:54:46 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 15:10:28 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	free_command_struct(t_data data)
{
	t_list	*tmp;

	tmp = data.command->file;
	while (tmp)
	{
		data.file_data = tmp->content;
		free(data.file_data->path);
		free(data.file_data->path_2);
		tmp = tmp->next;
	}
	free_list(&data.command->prototype);
	free_list(&data.command->file);
	free(data.command);
}

int	is_plus_sign(t_data *data, char *var, int i)
{
	if (var[i] == '+' && i == (int)ft_strlen(var) - 1 && data->info->value)
		return (1);
	return (0);
}

char	**env_array(t_data *data)
{
	char	**envp;
	int		i;

	i = 0;
	envp = malloc(sizeof(*envp) * (ft_lstsize(data->exported) + 1));
	while (data->exported)
	{
		data->info = data->exported->content;
		envp[i++] = ft_strjoin_and_free
			(ft_strjoin(data->info->var, "="), data->info->value, 1);
		data->exported = data->exported->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	close_fds(t_data *data)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (data->fd[i] != ERROR)
			close(data->fd[i]);
	}
	if (data->end[0] != ERROR)
		close(data->end[0]);
	if (data->end[1] != ERROR)
		close(data->end[1]);
}

void	close_fds_and_wait(t_data *data)
{
	int		stat;
	t_list	*tmp;

	close_fds(data);
	tmp = data->lst_child_id;
	while (data->lst_child_id)
	{
		data->process = data->lst_child_id->content;
		waitpid(data->process->id, &stat, 0);
		data->lst_child_id = data->lst_child_id->next;
	}
	if (WIFEXITED(stat))
		g_shell.exit_status = WEXITSTATUS(stat);
	data->lst_child_id = tmp;
	free_list(&data->lst_child_id);
}
