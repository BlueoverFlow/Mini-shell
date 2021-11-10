/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 13:25:34 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/11/07 13:48:01 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

void	pwd(char *ptr)
{
	char	*path;
	char	*tmp;

	ptr = NULL;
	path = malloc(100);
	getcwd(path, 1024);
	tmp = path;
	if (g_main.point)
	{
		path = ft_strjoin(path, g_main.point);
		free(tmp);
	}
	printf("%s\n", path);
	free(path);
}
