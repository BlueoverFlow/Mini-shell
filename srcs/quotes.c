/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 13:31:16 by ael-mezz         ###   ########.fr       */
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
		if (str[i] == '\'' || str[i] == '"')
		{
			if (data->pos[0] != ERROR && str[i] != str[data->pos[0]])
				continue ;
			if (!(str[i] == '"' && is_backslashed(i, str)))
				data->pos[j++] = i;
		}
}

static void unquoted_field(t_data *data, char *blanks, t_list *last)
{
	BOOL	next_field;
	char	**table;
	int		i;
	size_t	l;

	next_field = TRUE;
	l = ft_strlen(data->field);
	if (data->field[0] != blanks[0] && data->field[0] != blanks[1]
		&& data->is_one_token && ft_lstsize(data->tokens))
		data->is_one_token = TRUE;
	else
		data->is_one_token = FALSE;
	if ((data->field[l - 1] == blanks[0] && !is_backslashed(l - 1, data->field))
	 	|| (data->field[l - 1] == blanks[1] && !is_backslashed(l - 1, data->field)))
		next_field = FALSE;
	table = ft_split_input(data->field, "\t ");
	if (table)
		data->is_separated = TRUE;
	i = -1;
	if (data->is_one_token)
		last->content = ft_strjoin(last->content, table[1 + i++]);
	while (table[++i])
		ft_dlstadd_back(&data->tokens, ft_lstnew(table[i]));
	data->is_one_token = next_field;
}

static void merge_tokens(t_data *data, char *blanks)
{
	t_list *last;

	last = ft_lstlast(data->tokens);
	if (QUOTED_FIELD)
	{
		if ((ft_lstsize(data->tokens) && (((char *)(last->content))[0] == '\''
			|| ((char *)(last->content))[0] == '\"') && !data->is_separated)
			|| data->is_one_token)
			last->content = ft_strjoin(last->content, data->field);
		else
			ft_dlstadd_back(&data->tokens, ft_lstnew(data->field));
		data->is_separated = FALSE;
		data->is_one_token = TRUE;
	}
	else
		unquoted_field(data, blanks, last);
}

int to_tokens(t_data *data, char *input)
{
	int i;
	char blanks[2] = {' ', '\t'};

	i = 0;
	while (input[i])
	{
		next_field(input + i, data);
		if (!data->is_quoted && NEXT_IS_UNCLOSED)
		{
			out(0, *data);
			return (0);
		}
		if (!data->pos[0])
			data->is_quoted = TRUE;
		if (!data->is_quoted)
		{
			if (data->pos[0] == ERROR)
				data->field = ft_strdup2(input + i);
			else
				data->field = ft_substr(input, i, data->pos[0]);
		}
		else
		{
			data->field = ft_substr(input, i, data->pos[1] + 1);
			data->is_quoted = FALSE;
		}
		merge_tokens(data, blanks);
		i += ft_strlen(data->field);
	}
	return (1);
}