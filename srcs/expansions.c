/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 21:08:11 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void single_quote_exp(t_data *data, char *new)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (data->token[++i])
		if (data->token[i] != '\'')
		{
			if (new)
				new[j] = data->token[i];
			j++;
		}
	if (new)
	{
		new[j] = '\0';
		free(data->token);
		data->token = new;
		return ;
	}
	new = malloc(sizeof(char) * (j + 1));
	if (!new)
		out(1, *data);
	single_quote_exp(data, new);
}

/*
still need to expand environment
*/
static void double_quote_exp(t_data *data, char *new)
{
	int		i;
	int		j;
	char	special[3] = {'\\', '"', '$'};

	i = 0;
	j = 0;
	while (data->token[++i])
	{
		if (data->token[i + 1]
			&& is_backslashed(i + 1, data->token)
			&& (data->token[i] == special[0]
				|| data->token[i] == special[1]))
			continue ;
		if (new)
			new[j] = data->token[i];
		j++;
	}
	if (new)
	{
		new[--j] = '\0';
		free(data->token);
		data->token = new;
		return ;
	}
	new = malloc(sizeof(char) * j);
	if (!new)
		out(1, *data);
	double_quote_exp(data, new);
}

static void expand_input(t_data *data, t_list *last)
{
	char	**table;
	int		i;

	//need to expand
	
	table = ft_split_input(data->token, "\t ");
	i = -1;
	if (data->merge && table[i + 1])
		last->content = ft_strjoin(last->content, table[1 + i++]);
	while (table[++i + 1])
		ft_dlstadd_back(&data->fields, ft_lstnew(table[i]));
}

void expand_token(t_data *data)
{
	int		*i;
	t_list	*last;
	char	blanks[2] = {' ', '\t'};

	last = ft_lstlast(data->fields);
	pre_check(data, blanks);
	if (data->token[0] == '\'')
		single_quote_exp(data, NULL);
	else if (data->token[0] == '"')
		double_quote_exp(data, NULL);
	else
		expand_input(data, last);
	merge_token(data, last);
}