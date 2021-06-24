/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:31:19 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/24 13:34:08 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int env(t_data *data, char **prototype)
{
	t_list	*tmp;
	int i;
	int j;

	i = -1;
	tmp = data->exported;
	if (prototype[1])
		return (out(data, "unsupported syntax!\n", 1));
	while (data->envp[++i])
		if (!data->exported)
			printf("%s\n", data->envp[i]);
	if (!data->exported)
		return (1);
	i--;
	j = 0;
	while (j++ < i / 2)
		tmp = tmp->next;
	while (tmp)
	{
		printf("%s\n", tmp->content + 11);
		tmp = tmp->previous;
	}
	tmp = ft_lstlast(data->exported);
	while (i-- > j)
	{
		printf("%s\n", tmp->content + 11);
		tmp = tmp->previous;
	}
	return (1);
}