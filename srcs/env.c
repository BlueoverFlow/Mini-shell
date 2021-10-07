/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:31:19 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/05 12:23:27 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env(t_data *data, char **prototype)
{
	t_list	*tmp;
	int		i;
	int		j;

	i = -1;
	if (prototype[1])
		return (error_msg(data, "unsupported syntax!\n", 1));
	if (!data->exported)
		export(data, NULL);
	tmp = data->exported;
	while (tmp)
	{
		printf("%s\n", tmp->content + 11);
		tmp = tmp->next;
	}
	return (1);
}
