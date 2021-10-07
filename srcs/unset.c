/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 11:44:55 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/07 08:39:46 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int check_syntax(t_data *data, char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[0]) && var[i] != '_')
		return (ERROR);
	while (var[++i])
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (ERROR);
	return (1);
}

int	unset(t_data *data, char **prototype)
{
    int		i;

	data->unset_cmd = TRUE;
    if (!data->exported)
		export(data, NULL);
    i = 0;
	while (prototype[++i] && prototype[i][0])
	{
		if (check_syntax(data, prototype[i]) == ERROR)
			return (error_msg(data, prototype[i], EXPORT_ERR));
		scan_env_vars(data, &prototype[i], NULL);
	}
	data->unset_cmd = FALSE;
    return (1);
}