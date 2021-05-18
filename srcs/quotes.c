/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/18 17:21:30 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void next_field(char *str, t_data *data)
{
	int i;
	int j;

	i = -1;
	j = 0;
	data->pos[0] = ERROR;
	data->pos[1] = ERROR;
	while (str[++i] && j < 2)
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (data->pos[0] != ERROR && str[i] != str[data->pos[0]])
				continue ;
			if (str[i - 1] != '\\')
				data->pos[j++] = i;
		}
}

static void merge_tokens(t_data *data, char *blanks)
{
	t_list *last;
	int		l;

	l = -1;
	last = ft_lstlast(data->tokens);
	if (QUOTED_FIELD)
	{
		if (data->is_one_token)
			last->content = ft_strjoin2(last->content, data->field);
		else
			ft_dlstadd_back(&data->tokens, ft_lstnew(data->field));
		if (data->field[ft_strlen(data->field) - 1] != blanks[0] && (data->field[ft_strlen(data->field) - 1] != blanks[1]))
			data->is_one_token = TRUE;
	}
	else
	{
		data->sub_field = ft_split(data->field, ' ');
		if (NEED_MERGE)
		{
			last->content = ft_strjoin2(last->content, data->sub_field[++l]);
			if (data->field[ft_strlen(data->sub_field[l]) - 1] == blanks[0] || (data->field[ft_strlen(data->sub_field[l]) - 1] == blanks[1]))
			data->is_one_token = FALSE;
		}
		if (NO_MERGE)
		{
			--l;
			while (data->sub_field[++l])
				ft_dlstadd_back(&data->tokens, ft_lstnew(data->sub_field[l]));
			if (data->field[ft_strlen(data->field) - 1] == blanks[0] || (data->field[ft_strlen(data->field) - 1] == blanks[1]))
				data->is_one_token = FALSE;
			else
			data->is_one_token = TRUE;	
		}
	}
}

int to_tokens(t_data *data)
{
	int i;
	char blanks[2] = {' ', '\t'};

	i = -1;
	while (data->input[++i])
	{
		next_field(data->input + i, data);
		if (!data->is_quoted && NEXT_IS_UNCLOSED)
		{
			out(0);
			return (0);
		}
		if (!data->pos[0])
			data->is_quoted = TRUE;
		if (!data->is_quoted)
		{
			if (data->pos[0] == ERROR)
				data->field = ft_strdup2(data->input + i);
			else
				data->field = ft_substr(data->input, i, data->pos[0]);
			data->is_quoted = TRUE;
		}
		else
		{
			data->field = ft_substr(data->input, i, data->pos[1] + 1);
			data->is_quoted = FALSE;
		}
		merge_tokens(data, blanks);
		i += ft_strlen(data->field) - 1;
	}
	return (1);
}