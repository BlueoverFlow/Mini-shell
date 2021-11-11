/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 13:25:34 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/11/11 10:17:35 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	pwd(t_data data)
{
	char	*path;

	if (data.prototype[1] && *data.prototype[1] == '-')
		return (error_msg(data, M_STXERR, 1, data.prototype[1]));
	path = getcwd(NULL, 0);
	printf("%s\n", path);
	free(path);
	return (EXIT_SUCCESS);
}
