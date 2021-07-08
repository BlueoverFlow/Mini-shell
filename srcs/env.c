/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:31:19 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/07/02 17:20:58 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env(t_data *data, char **prototype)
{
	t_list	*tmp;
	int		i;
	int		j;

	i = -1;
	tmp = data->exported;
	if (prototype[1])
		return (out(data, "unsupported syntax!\n", 1));
	if (!data->exported)
		export(data, NULL);
	while (data->exported)
		printf("%s + 11", data->exported->content);
	return (1);
}
