/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fields.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/08 19:03:24 by ael-mezz         ###   ########.fr       */
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
static void pre_check(t_data *data, char *blanks)
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
		if (data->token[0] != blanks[0] && data->token[0] == blanks[1] && !data->is_separated)
			data->merge = TRUE;
		if ((data->token[l - 1] == blanks[0] && !is_backslashed(l - 1, data->token))
			|| (data->token[l - 1] == blanks[1] && !is_backslashed(l - 1, data->token)))
			data->is_separated = TRUE;
		else
			data->is_separated = FALSE;
	}
}

static void merge_unquoted_token(t_data *data, t_list *last)
{
	char	**table;
	int		i;

	table = ft_split_input(data->token, "\t ");
	i = -1;
	if (data->merge && table[i])
		last->content = ft_strjoin(last->content, table[1 + i++]);
	while (table[++i])
		ft_dlstadd_back(&data->fields, ft_lstnew(table[i]));
}

void merge_quoted_token(t_data *data, t_list *last)
{
	if (data->merge)
		last->content = ft_strjoin(last->content, data->token);
	else
		ft_dlstadd_back(&data->fields, ft_lstnew(data->token));
}

static merge_tokens(t_data *data)
{
	int		*i;
	t_list	*last;
	char	blanks[2] = {' ', '\t'};

	last = ft_lstlast(data->fields);
	pre_check(data, blanks);
	if (QUOTED_TOKEN)
		merge_quoted_token(data, last);
	else
		merge_unquoted_token(data, last);
}

static int find_chrs(char *str, char* chars)
{
	int i;
	int j;

	i = -1;
	if (str)
		while (str[++i])
		{
			j = -1;
			if (chars)
				while (chars[++j])
					if (str[i] == chars[j])
						return (i);
		}
	return (i);
}

static int check_redirections(t_list **field, t_data *data)
{
	int		i;
	char	*file_path;
	int	rsl;

	i = -1;
	rsl = find_chr((*field)->content, ft_strdup("<>"));
	if (rsl != ERROR)
	{
		ft_dlstadd_back(&data->prototype, ft_lstnew(ft_substr((*field)->content, 0, rsl + 1)));
		
	}
	return (FALSE);
}

static int tokens_analyser(t_data *data)
{
	t_list	*tmp_field;
	int		rsl;

	merge_tokens;
	tmp_field = data->fields;
	while (tmp_field)
	{
		rsl = check_redirections(&tmp_field, data);
		if (rsl)
		{
			if (rsl == ERROR)
				return (ERROR);	
			tmp_field = tmp_field->next;
		}
		ft_dlstadd_back(&data->prototype, ft_lstnew(tmp_field->content));
		tmp_field = tmp_field->next;
	}
}

int extract_branches(t_data *data, char *input)
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
			return (ERROR);
		}
		if (!data->pos[0])
			j++;
		if (data->pos[0] == ERROR)
			data->token = ft_strdup(input + i);
		else
			data->token = ft_substr(input, i, data->pos[j] + j);
		i += ft_strlen(data->token);
		tokens_analyser(data);
	}
	return (1);
}