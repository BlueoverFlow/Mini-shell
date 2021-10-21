/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:31:19 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/21 08:32:56 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	env(t_data *data)
{
	t_list	*tmp;

	if (data->prototype[1] && data->prototype[1][0])
		return (error_msg(data, M_STXERR, NULL));
	tmp = data->exported;
	while (data->exported)
	{
		data->info = data->exported->content;
		if (data->info->value && *(data->info->value))
			printf("%s=%s\n", data->info->var, data->info->value);
		data->exported = data->exported->next;
	}
	data->exported = tmp;
	return (EXIT_SUCCESS);
}
