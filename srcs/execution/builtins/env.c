/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:31:19 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/19 13:01:33 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	env(t_data data)
{
	if (data.prototype[1] && data.prototype[1][0])
		return (error_msg(data, M_STXERR, 1, NULL));
	while (data.exported)
	{
		data.info = data.exported->content;
		if (data.info->value && *(data.info->value))
			printf("%s=%s\n", data.info->var, data.info->value);
		data.exported = data.exported->next;
	}
	return (EXIT_SUCCESS);
}
