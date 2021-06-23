/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/23 19:22:36 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_special_char(int i, char *input, char *special)
{
	int j;

	j = -1;
	while (special[++j] != '\0')
		if (input[i] == special[j])
			return (1);
	return (0);
}

char	*expand_token(t_data *data, char *input)
{
	int j;
	int i;
	char special[4] = {' ', '\t', '\0'};
	char special_2[3] = {'"', '\0'};
	char *new;
	
	j = 0;
	data->quoting_state = UNQUOTED;
	new = ft_calloc(ft_strlen(input) + 1, sizeof *new);
	if (!new)
		out(data, "ALlocation failure!\n", 1);
	i = -1;
	while (input[++i])
	{
		define_quoting_state(data, input, i);
		if ((data->quoting_state == UNQUOTED && is_special_char(i, input, special))
			|| (data->quoting_state == '"' && is_special_char(i, input, special_2))
			|| (data->quoting_state == '\'' && input[i] == '\''))
				continue ;
		new[j++] = input[i];
	}
	free(input);
	return (new);
}
