/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 11:19:40 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/24 12:22:14 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	ft_countwords(char const *s)
{
	int		start;
	size_t	count;

	count = 1;
	start = 0;
	while (s[start])
	{
		while (s[start] && ((' ' == s[start] && !is_backslashed(start, (char *)s)) || ('\t' == s[start] && !is_backslashed(start, (char *)s))))
			start++;
		if (s[start])
			count++;
		while (s[start] &&((' ' != s[start] && !is_backslashed(start, (char *)s)) && ('\t' != s[start] && !is_backslashed(start, (char *)s))))
			start++;
	}
	return (count);
}

static char	**ft_koalloc(char **ptr, size_t i)
{
	i += 1;
	while (ptr[--i] != NULL)
		free(ptr[i]);
	free(ptr);
	return (NULL);
}

static char	**ft_split2(char const *s, char **ptr, size_t cw)
{
	size_t	i;
	int		start;
	int		len;

	i = -1;
	start = 0;
	len = 0;
	while (s[start] && ++i < cw - 1)
	{
		while ((' ' == s[start] && !is_backslashed(start, (char *)s)) || ('\t' == s[start] && !is_backslashed(start, (char *)s)))
			start++;
		while ((' ' != s[start] && !is_backslashed(start, (char *)s)) && ('\t' != s[start] && !is_backslashed(start, (char *)s)))
		{
			start++;
			len++;
		}
		start -= len;
		ptr[i] = ft_calloc(len + 1, sizeof(char));
		if (!ptr[i])
			return (ft_koalloc(ptr, i));
		ft_memcpy(ptr[i], s + start, len);
		start += len;
		len = 0;
	}
	return (ptr);
}

char	**ft_split_blanks(char const *s)
{
	char		**ptr;
	size_t		cw;

	if (s == NULL)
		return (NULL);
	cw = ft_countwords(s);
	ptr = (char **)malloc(sizeof(char *) * cw);
	if (!ptr)
		return (NULL);
	ptr[cw - 1] = NULL;
	return (ft_split2(s, ptr, cw));
}
