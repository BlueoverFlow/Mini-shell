/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 16:59:42 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/30 18:02:44 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	increase_shelllvl(t_data *data)
{
	t_list	*tmp;
	int		level;

	tmp = data->exported;
	while (data->exported)
	{
		data->info = data->exported->content;
		if (!ft_strcmp(data->info->var, "SHLVL"))
		{
			if (data->info->value)
			{
				level = ft_atoi(data->info->value) + 1;
				free(data->info->value);
				data->info->value = ft_itoa(level);
			}
		}
		data->exported = data->exported->next;
	}
	data->exported = tmp;
}

void	export_print(t_data *data)
{
	t_list	*tmp;

	tmp = data->exported;
	while (tmp)
	{
		data->info = tmp->content;
		if (data->info->value)
			printf("declare -x %s=\"%s\"\n", data->info->var, data->info->value);
		else
			printf("declare -x %s\n", data->info->var);
		tmp = tmp->next;
	}
}

static void	sort_var(t_data *data)
{
	t_list			*last;
	t_info			*info_1;

	last = ft_lstlast(data->exported);
	data->info = last->content;
	while (last->previous)
	{
		info_1 = last->previous->content;
		if (ft_strcmp(data->info->var, info_1->var) > 0)
			break ;
		if (last->previous->previous)
			last->previous->previous->next = last;
		last->previous->next = last->next;
		last->next = last->previous;
		last->previous = last->next->previous;
		if (last->next->next)
			last->next->next->previous = last->next;
		last->next->previous = last;
	}
	if (!last->previous)
		data->exported = last;
}

void	insert_var(t_data *data, char *input)
{
	int				i;

	if (input)
	{
		i = find_char(input, '=');
		data->info = malloc(sizeof(t_info));
		data->info->var = ft_substr(input, 0, i);
		data->info->value = ft_substr(input, i + 1, ft_strlen(input) - i);
	}
	ft_dlstadd_back(&data->exported, ft_dlstnew(data->info));
	sort_var(data);
}

void	build_env_vars(t_data *data, char *const	*envp)
{
	int	i;

	i = -1;
	data->exported = NULL;
	while (envp[++i])
		insert_var(data, (char *)envp[i]);
	increase_shelllvl(data);
}
