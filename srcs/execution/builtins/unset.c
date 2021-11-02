/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 11:44:55 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/02 12:39:48 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static int	check_syntax(t_data *data, char *var)
{
	int	i;

	i = 0;
	data->var_with_equals_sign = FALSE;
	if (!ft_isalpha(var[0]) && var[i] != '_')
		return (ERROR);
	while (var[++i])
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (ERROR);
	return (1);
}

static t_list	*skip(t_list *lst, t_list *head)
{
	if (head == lst)
		head = lst->next;
	if (lst->previous)
		lst->previous->next = lst->next;
	if (lst->next)
		lst->next->previous = lst->previous;
	return (head);
}

int	unset(t_data *data)
{
	int		i;
	t_list	*tmp;

	i = 0;
	while (data->prototype[++i] && data->prototype[i][0])
	{
		if (check_syntax(data, data->prototype[i]) == ERROR)
			return (error_msg(data, M_NOVALID, data->prototype[i]));
		tmp = data->exported;
		while (tmp)
		{
			data->info = tmp->content;
			if (!ft_strcmp(data->info->var, data->prototype[i]))
			{
				data->exported = skip(tmp, data->exported);
				free(data->info->value);
				free(data->info->var);
				free(data->info);
				free(tmp);
				break ;
			}
			tmp = tmp->next;
		}
	}
	return (EXIT_SUCCESS);
}
