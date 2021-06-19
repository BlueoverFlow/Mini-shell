/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 09:41:53 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/17 13:54:00 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int out(int code, t_data data)
{
	if (code == 0)
		ft_putstr_fd("syntax error!\n", STDERR_FILENO);
	if (code == 1)
	{
		/* clear the memory */
		ft_putstr_fd("Allocation error!\n", STDERR_FILENO);
		exit(1);
	}
	/* clear the memory */
	return (ERROR);
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

static int separator_search(const char *s, char *separator, int c)
{
	int i = -1;
	int j = 0;

	while (separator[++i])
	{
		if ((s[c] == separator[i] && is_backslashed(c, (char *)s)) || s[c] != separator[i])
			j++;
	}
	if (j == ft_strlen(separator))
		return (0);
	return (1);
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
		while (separator_search(s, separator, start) && s[start])
			start++;
		if (s[start])
			count++;
		while (!separator_search(s, separator, start) && s[start])
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

static char	**spliter(char const *s, char **ptr, char *separator, size_t cw)
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
		while (separator_search(s, separator, start) && s[start])
			start++;
		j = -1;
		while (!separator_search(s, separator, start) && s[start])
		{
			len++;
			start++;
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
	return (spliter(s, ptr, separator,cw));
}

BOOL quoted_fragment(char c)
{
	if (c == '\'' || c == '\"')
		return (TRUE);
	return (FALSE);
}