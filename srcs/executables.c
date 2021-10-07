/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 09:57:52 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/10/05 12:44:28 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*jointhreestr(char *path, char *bslash, char *cmd)
{
	char	*joinpathslash;
	char	*tmp;

	joinpathslash = ft_strjoin(path, bslash);
	tmp = joinpathslash;
	joinpathslash = ft_strjoin(joinpathslash, cmd);
	free(tmp);
	return (joinpathslash);
}

int	run_executable(t_data *data ,char *prototype)
{
	char	*path;
	char	**r;
	int		i;
	char	**k;
	int		j;

	i = 0;
	j = 0;
	path = getenv("PATH");
	r = ft_split(path, ':');
	while (r[i])
		ft_strjoin(r[i++], "/");
	k = (char **)malloc(sizeof(char *) * (i + 1));
	while (j < i)
	{
		k[j] = jointhreestr(r[j], "/", prototype);
		k[j + 1] = NULL;
		if(!(execve(k[j], &k[j], NULL)))
			return(0);
		free(k[j]);
		j++;
	}
	return (0);
}
