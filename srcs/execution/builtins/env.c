/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:31:19 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 09:38:51 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	env(t_data *data, char **prototype)
{
	t_list	*tmp;

	if (prototype[1] && *prototype[1])
		return (error_msg(data, "syntax error!\n", NORMAL_ERR));
	tmp = data->exported;
	while (data->exported)
	{
		data->info = data->exported->content;
		if (data->info->value && *(data->info->value))
			printf("%s=%s\n", data->info->var, data->info->value);
		data->exported = data->exported->next;
	}
	data->exported = tmp;
	return (1);
}
