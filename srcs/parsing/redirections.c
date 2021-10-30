/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:33:54 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/30 15:39:35 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	assign_redirection_id(t_data *data, char *fragment)
{
	if (fragment[0] == '>')
	{
		if (fragment[1] && fragment[1] == '>')
			data->file_data->id = APPENDED_REDIRECTED_OUTPUT;
		else
			data->file_data->id = REDIRECTED_OUTPUT;
	}
	else if (fragment[0] == '<')
	{
		if (fragment[1] && fragment[1] == '<')
			data->file_data->id = HEREDOC;
		else
			data->file_data->id = REDIRECTED_INPUT;
	}
}

static	int	fill_file_id(t_data *data, char **fragment)
{
	if ((!data->command->file) || (data->command->file && !data->file_data))
	{
		data->file_data = malloc(sizeof(t_file_data));
		data->file_data->id = -1;
		data->file_data->path = NULL;
	}
	if (data->file_data && data->file_data->id != -1 && !data->file_data->path)
		return (EXIT_FAILURE);
	assign_redirection_id(data, *fragment);
	(*fragment)++;
	if (data->file_data->id == HEREDOC
		|| data->file_data->id == APPENDED_REDIRECTED_OUTPUT)
		(*fragment)++;
	ft_lstadd_back(&data->command->file, ft_lstnew(data->file_data));
	return (EXIT_SUCCESS);
}

static int	fill_file_path(t_data *data, char *token)
{
	if (data->command->file && data->file_data && !data->file_data->path)
	{
		token = expand_env_vars(data, token);
		token = expand_token(data, token);
		data->file_data->path = token;
		data->file_data = NULL;
		return (TRUE);
	}
	return (EXIT_SUCCESS);
}

int	hundle_redirection(t_data *data, char *fragment, char *token, int i)
{
	t_list	*last;
	int		ret;

	last = ft_lstlast(data->command->file);
	if (fragment[i] == '>' || fragment[i] == '<')
	{
		if (fill_file_id(data, &fragment))
		{
			free(token);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		ret = fill_file_path(data, token);
		if (ret == EXIT_FAILURE)
			return (ret);
		else if (!ret)
			ft_lstadd_back(&data->command->prototype, ft_lstnew(token));
		fragment += i;
	}
	return (make_branch(data, fragment));
}
