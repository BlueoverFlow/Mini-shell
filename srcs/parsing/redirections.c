/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:33:54 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 09:28:13 by ael-mezz         ###   ########.fr       */
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
		return (ERROR);
	assign_redirection_id(data, *fragment);
	(*fragment)++;
	if (data->file_data->id == HEREDOC
		|| data->file_data->id == APPENDED_REDIRECTED_OUTPUT)
		(*fragment)++;
	ft_lstadd_back(&data->command->file, ft_lstnew(data->file_data));
	return (1);
}

static int	fill_file_path(t_data *data, char *token)
{
	if (data->command->file && data->file_data && !data->file_data->path)
	{
		if (!ft_strncmp(token, "|", 2))
			return (error_msg(data, "syntax error near unexpected token `|'\n"
					, NORMAL_ERR));
		data->file_data->path = token;
		data->file_data = NULL;
		return (1);
	}
	return (0);
}

int	hundle_redirection(t_data *data, char *fragment, char *token, int i)
{
	t_list	*last;
	int		ret;

	last = ft_lstlast(data->command->file);
	if (fragment[i] && is_redirection(fragment, 0, UNQUOTED))
	{
		if (fill_file_id(data, &fragment) == ERROR)
			return (ERROR);
	}
	else
	{
		ret = fill_file_path(data, token);
		if (ret == ERROR)
			return (ret);
		else if (!ret)
			ft_lstadd_back(&data->command->prototype, ft_lstnew(token));
		fragment += i;
	}
	return (make_branch(data, fragment));
}
