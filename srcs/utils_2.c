/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 11:19:40 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/18 11:19:57 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *split_with_blanks(t_data *data, char *token, char *blanks)
{
	int i;
	int	j;

	i = -1;
	j = 0;
	while (data->field[++i])
	{
		if (data->field[i] != blanks[0] && data->field[i] != blanks[1])
		{
			if (token)
				token[j] = data->field[i];
			j++;
		}
		else
			if (j)
				break;
	}
	if (token)
	{
		token[j] = '\0';
		free(data->field);
		printf("-- token --: |%s|\n", token);
		return (token);
	}
	token = malloc(sizeof(char) * j + 1);
	split_with_blanks(data, token, blanks);
	return (token);
}