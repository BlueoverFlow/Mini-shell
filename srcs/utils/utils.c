/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 16:44:54 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/12 13:28:43 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

BOOL	theres_atoken(char *fragment)
{
	int	i;

	i = -1;
	if (fragment)
	{
		while (fragment[++i])
		{
			if (fragment[i] != ' ')
				return (TRUE);
		}
		free(fragment);
	}
	return (FALSE);
}

BOOL	is_redirection(t_data *data, char *str, int i)
{
	define_quoting_state(data, str, i);
	if ((str[i] == '>' || str[i] == '<') && data->quoting_state == UNQUOTED)
		return (TRUE);
	return (FALSE);
}

BOOL	closed_quotes(char *input, int i)
{
	int	j;

	j = i;
	while (input[++j])
	{
		if (input[j] == input[i])
			return (TRUE);
	}
	return (FALSE);
}

void	define_quoting_state(t_data *data, char *input, int i)
{
	if (data->passive)
	{
		data->quoting_state = UNQUOTED;
		data->passive = FALSE;
	}
	else if (input[i] == data->quoting_state)
		data->passive = TRUE;
	if (data->quoting_state == UNQUOTED && (input[i] == '"' || input[i] == '\'')
		&& closed_quotes(input, i))
		data->quoting_state = input[i];
}

char	*lst_to_word(t_list *lst)
{
	int		l;
	char	*str;
	int		i;

	i = 0;
	l = ft_lstsize(lst);
	if (!l)
		return (NULL);
	str = malloc(sizeof(*str) * (l + 1));
	while (lst)
	{
		str[i++] = *(char *)lst->content;
		lst = lst->next;
	}
	str[i] = '\0';
	return (str);
}
