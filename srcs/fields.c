/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fields.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 21:28:09 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void next_token(char *str, t_data *data)
{
	int i;
	int j;

	i = -1;
	j = 0;
	data->pos[0] = ERROR;
	data->pos[1] = ERROR;
	while (str[++i] && j < 2)
		if (str[i] == '\'' || str[i] == '"')
		{
			if (data->pos[0] != ERROR && str[i] != str[data->pos[0]])
				continue ;
			if (!(str[i] == '"' && is_backslashed(i, str)))
				data->pos[j++] = i;
		}
}

void merge_token(t_data *data, t_list *last)
{
	if (data->merge)
		last->content = ft_strjoin(last->content, data->token);
	else
		ft_dlstadd_back(&data->fields, ft_lstnew(data->token));
}

void pre_check(t_data *data, char *blanks)
{
	BOOL	next_token;
	char	**table;
	int		i;
	size_t	l;

	data->merge = FALSE;
	l = ft_strlen(data->token);
	if (QUOTED_TOKEN)
	{
		if (!data->is_separated)
			data->merge = TRUE;
		data->is_separated = FALSE;
	}
	else
	{
		if ((data->token[0] != blanks[0] && data->token[0] != blanks[1])
			|| data->is_separated)
			data->merge = FALSE;
		else
			data->merge = TRUE;
		if ((data->token[l - 1] == blanks[0] && !is_backslashed(l - 1, data->token))
			|| (data->token[l - 1] == blanks[1] && !is_backslashed(l - 1, data->token)))
			data->is_separated = TRUE;
		else
			data->is_separated = FALSE;
	}
}

int extract_fields(t_data *data, char *input)
{
	int i;
	int j;

	i = 0;
	data->token = NULL;
	while (input[i])
	{
		j = 0;
		next_token(input + i, data);
		if (NEXT_IS_UNCLOSED)
		{
			out(0, *data);
			return (0);
		}
		if (!data->pos[0])
			j++;
		if (data->pos[0] == ERROR)
			data->token = ft_strdup(input + i);
		else
			data->token = ft_substr(input, i, data->pos[j] + j);
		i += ft_strlen(data->token);
		expand_token(data);
	}
	return (1);
}