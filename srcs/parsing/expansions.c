/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/18 12:04:45 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	skip_characters(t_data *data, char *input, int i)
{
	if ((data->quoting_state == UNQUOTED
			&& (input[i] == ' ' || input[i] == '\t'))
		|| (data->quoting_state == '"'
			&& input[i] == '\"')
		|| (data->quoting_state == '\'' && input[i] == '\''))
		return (TRUE);
	return (FALSE);
}

char	*expand_token(t_data *data, char *input)
{
	char	*new;
	int		j;
	int		i;

	j = 0;
	i = -1;
	if (!input)
		return (NULL);
	data->quoting_state = UNQUOTED;
	new = ft_calloc(ft_strlen(input) + 1, sizeof(*new));
	while (input && input[++i])
	{
		define_quoting_state(data, input, i);
		if (skip_characters(data, input, i))
			continue ;
		new[j++] = input[i];
	}
	free(input);
	return (new);
}
