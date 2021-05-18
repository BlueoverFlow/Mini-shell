/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 09:41:53 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/18 16:24:22 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void out(int code)
{
	if (code == 0)
		ft_putstr_fd("Error while parsing!\n", STDERR_FILENO);
}

static int		len(char *s)
{
	int i;
	int c;

	i = -1;
	c = 1;
	while (s[++i])
	{
		if (s[i] == ' ' || s[i] == '\t')
		{
			if (s[i + 1] != '\0' && s[i + 1] != ' '
					&& s[i + 1] != '\t')
				c++;
		}
	}
	return (c);
}

char	**ft_split_blanks(char *s)
{
	char	**tab;
	int		i;
	int		j;
	int		sp;
	char	*str;

	i = -1;
	j = 0;
	sp = 1;
	if (!s)
		return (NULL);
	str = ft_strdup2(s);
	if (!(tab = (char**)malloc(sizeof(char*) * (len(str) + 1))))
		return (0);
	while (str[++i])
	{
		if (sp && str[i] != ' ' && str[i] != '\t')
		{
			tab[j++] = &str[i];
			sp = 0;
		}
		else if (str[i] == ' ' || str[i] == '\t')
		{
			str[i] = '\0';
			sp = 1;
		}
	}
	free(str);
	tab[j] = 0;
	return (tab);
}
