/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@sudent.1337.ma>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/19 16:07:35 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	check_ifn(char **args)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (args[++i])
	{
		j = 0;
		if (args[i][j++] == '-' && args[i][j] && args[i][j] == 'n')
		{
			while (args[i][j] == 'n')
				j++;
			if (args[i][j] && args[i][j] != 'n')
				return (1);
		}
		else
			return (i);
	}
	return (i); 
}

static int     echo(char **args)
{
	size_t	i;
	int		n;
	t_var	d;

	n = 1;
	if (d._error != 1)
		d._status = 0;
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return (1);
	}
	if (!ft_strcmp(args[1], " ") && !args[2])
		return (1);
	i = check_ifn(args);
	// if(i > 1)
	// 	n = 0;
	// else
	// 	n = 1;
	n = i > 1 ? 0 : 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		i++;
		if (args[i])
			ft_putchar_fd(' ', 1);
	}
	if (n)
		ft_putchar_fd('\n', 1);
	return (1);
}

static char **lst_to_table(t_list *lst)
{
	char	**table;
	int		i;

	table = malloc(sizeof *table * (ft_lstsize(lst) + 1));
	i = 0;
	while (lst)
	{
		table[i++] = ft_strdup(lst->content);
		lst = lst->next;
	}
	table[i] = NULL;
	return (table);
}

int is_builtin(t_data *data)
{
	char **prototype;

	prototype = lst_to_table(data->prototype);
	if (!ft_strcmp(prototype[0], "echo") || !ft_strcmp(prototype[0], "export"))
		echo(prototype);
	return (1);
}