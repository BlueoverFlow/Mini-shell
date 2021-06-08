/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/08 18:10:51 by ael-mezz         ###   ########.fr       */
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

	i = -1;
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
		new[j] = '\0';
		free(data->token);
		data->token = new;
		return ;
	}
	new = malloc(sizeof(char) * (j + 1));
	if (!new)
		out(1, *data);
	double_quote_exp(data, new);
}

