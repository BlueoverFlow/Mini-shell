/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 16:01:49 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int data_tree(t_data *data)
{
	char **cmd;
	char **pipe_line;
	int i, j;
	
	cmd = ft_split_input(data->input, ft_strdup2(";"));
	i = -1;
	while (cmd[++i])
	{
		pipe_line = ft_split_input(cmd[i], ft_strdup2("|"));
		j = -1;
		while (pipe_line[++j])
		{ 
			if (!parser(data, 0, pipe_line[j]))
				return (0);
		}
		ft_dlstadd_back(&data->commands, ft_lstnew(data->piped));
		printf(" size: data->piped: %i\n", ft_lstsize(data->piped));
		data->piped = NULL;
	}
	ft_dlstadd_back(&data->line, ft_lstnew(data->commands));
	printf(" size: data->commads: %i\n", ft_lstsize(data->commands));
	data->commands = NULL;
	return (1);
}

int	parser(t_data *data, int i, char *input)
{
	t_list	*node;
	t_list	*piped_tmp;
	char	**cmd;
	
	if (!to_tokens(data, input))
	{
		/* clear the memory */
		return (0);
	}
	analyse_tokens(data);
	ft_dlstadd_back(&data->piped, ft_lstnew(data->tokens));
	data->tokens = NULL;
	return (1);
}
