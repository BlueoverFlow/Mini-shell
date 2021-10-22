/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:29:24 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/22 13:36:27 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static int	is_plus_sign(t_data *data, char *var, int i)
{
	if (var[i] == '+' && i == (int)ft_strlen(var) - 1 && data->info->value)
		return (1);
	return (0);
}

static int	analyze_var(t_data *data, char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[0]) && var[i] != '_')
		return (ERROR);
	while (var[++i])
		if (!ft_isalnum(var[i]) && !is_plus_sign(data, var, i) && var[i] != '_')
			return (ERROR);
	return (1);
}

static int	check_export_syntax(t_data *data, int j)
{
	int	i;
	int	l;

	data->info = malloc(sizeof(t_info));
	l = ft_strlen(data->prototype[j]);
	i = find_char(data->prototype[j], '=');
	if (i == ERROR)
	{
		i = l;
		data->info->value = NULL;
	}
	else if (i == l - 1)
		data->info->value = ft_strdup("");
	else
		data->info->value
			= ft_substr(data->prototype[j], i + 1, l - i);
	data->info->var = ft_substr(data->prototype[j], 0, i);
	if (analyze_var(data, data->info->var) == ERROR)
		return (ERROR);
	return (1);
}

static void	export_print(t_data *data)
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

static void	insert_var(t_data *data, char *input)
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

static int	already_exported(t_data *data, int i, t_info *info_1)
{
	if (!ft_strcmp(info_1->var, data->info->var))
	{
		if (data->info->value)
		{
			if (i == ERROR)
			{
				free(info_1->value);
				info_1->value = NULL;
			}
			info_1->value
				= ft_strjoin_and_free_s1(info_1->value, data->info->value);
		}
		return (1);
	}
	return (0);
}

int	scan_env_vars(t_data *data)
{
	t_info	*info_1;
	t_list	*tmp;
	int		i;

	tmp = data->exported;
	i = find_char(data->info->var, '+');
	if (i != ERROR)
		data->info->var[ft_strlen(data->info->var) - 1] = '\0';
	while (data->exported)
	{
		info_1 = data->exported->content;
		if (already_exported(data, i, info_1))
		{
			data->exported = tmp;
			return (1);
		}
		data->exported = data->exported->next;
	}
	data->exported = tmp;
	return (0);
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

int	export(t_data *data)
{
	int		i;

	i = 0;
	if (!data->prototype[1] || !data->prototype[1][0])
		export_print(data);
	else if (*(data->prototype)[1] == '-')
		return (error_msg(data, M_STXERR, NULL));
	while (data->prototype[++i] && data->prototype[i][0])
	{
		if (check_export_syntax(data, i) == ERROR)
			return (error_msg(data, M_NOVALID, data->prototype[i]));
		if (!scan_env_vars(data))
			insert_var(data, NULL);
	}
	return (EXIT_SUCCESS);
}
