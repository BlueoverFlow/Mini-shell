/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 09:41:53 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 16:02:54 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void out(int code, t_data data)
{
	if (code == 0)
		ft_putstr_fd("Error while parsing!\n", STDERR_FILENO);
	if (code == 1)
	{
		free_list(&data.tokens);
		free_list(&data.garbage);
		ft_putstr_fd("Allocation error!\n", STDERR_FILENO);
		exit(1);
	}
}

int is_backslashed(int i, char *str)
{
	int	special;
	int	d;

	special = FALSE;
	if (i == 0)
		return (0);
	d = i - 1;
	while (d >= 0 && str[d] == '\\')
		d--;
	while (++d <= i - 1)
	{
		if (str[d] == '\\' && special == FALSE)
			special = TRUE;
		else if (str[d] == '\\' && special == TRUE)
			special = FALSE;
	}
	return (special);
}

static size_t	ft_countwords(char const *s, char *separator)
{
	int		start;
	size_t	count;
	int		i;

	count = 0;
	start = 0;
	while (s[start])
	{
		i = -1;
		while (separator[++i])
		{
			if (s[start] && s[start] == separator[i] && !is_backslashed(start, (char *)s))
			{
				start++;
				i = -1;
			}
		}
		if (s[start])
			count++;
		i = -1;
		while (separator[++i])
		{
			if (s[start] && (s[start] != separator[i]
				|| (s[start] == separator[i] && is_backslashed(start, (char *)s))))
			{
				start++;
				i = -1;
			}
		}
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

static char	**ft_split2(char const *s, char **ptr, char *separator, size_t cw)
{
	size_t	i;
	int		start;
	int		len;
	int		j;

	i = -1;
	start = 0;
	len = 0;
	while (s[start] && ++i < cw)
	{
		j = -1;
		while (separator[++j])
		{
			if (s[start] && s[start] == separator[j] && !is_backslashed(start, (char *)s))
			{
				start++;
				j = -1;
			}
		}
		j = -1;
		while (separator[++j])
		{
			if (s[start] && (s[start] != separator[j]
				|| (s[start] == separator[j] && is_backslashed(start, (char *)s))))
			{
				start++;
				len++;
				j = -1;
			}
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

char	**ft_split_input(char const *s, char *separator)
{
	char		**ptr;
	size_t		cw;

	if (s == NULL)
		return (NULL);
	cw = ft_countwords(s, separator);
	ptr = (char **)malloc(sizeof(char *) * (cw + 1));
	if (!ptr)
		return (NULL);
	ptr[cw] = NULL;
	return (ft_split2(s, ptr, separator,cw));
}
