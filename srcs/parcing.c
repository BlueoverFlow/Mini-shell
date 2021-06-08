/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/08 15:55:59 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int data_tree(t_data *data)
{
	char **cmd;
	char **pipe_line;
	int i, j;
	
	cmd = ft_split_input(data->input, ft_strdup(";"));
	i = -1;
	while (cmd[++i])
	{
		pipe_line = ft_split_input(cmd[i], ft_strdup("|"));
		j = -1;
		while (pipe_line[++j])
		{
			if (parser(data, 0, pipe_line[j]) == ERROR)
				return (ERROR);
		}
		ft_dlstadd_back(&data->commands, ft_lstnew(data->piped));
		data->piped = NULL;
	}
	ft_dlstadd_back(&data->line, ft_lstnew(data->commands));
	data->commands = NULL;
	return (1);
}

int	parser(t_data *data, int i, char *input)
{
	t_list	*node;
	t_list	*piped_tmp;
	char	**cmd;

	data->is_separated = TRUE;
	if (extract_branches(data, input) == ERROR)
		return (ERROR);
	ft_dlstadd_back(&data->piped, ft_lstnew(data->fields));
	data->fields = NULL;
	return (1);
}
