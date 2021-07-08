/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binarycmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 09:57:52 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/07/08 13:18:24 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*jointhreestr(char *path, char *bslash, char *cmd)
{
	char	*joinpathslash;
	char	*tmp;

	joinpathslash = ft_strjoin(path, bslash);
	tmp = joinpathslash;
	joinpathslash = ft_strjoin(joinpathslash, cmd);
	free(tmp);
	return (joinpathslash);
}

int	binarycmd(char *str)
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
		k[j] = jointhreestr(r[j], "/", str);
		k[j + 1] = NULL;
		if(!(execve(k[j], &k[j], NULL)))
			return(0);
		free(k[j]);
		j++;
	}
	return (0);
}
