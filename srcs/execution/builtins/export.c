/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:29:24 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 10:11:32 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

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
				= ft_strjoin_and_free(info_1->value, data->info->value, 1);
		}
		return (1);
	}
	return (0);
}

static int	scan_env_vars(t_data *data)
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
			if (ft_strcmp(data->info->value, info_1->value))
				free(data->info->value);
			free(data->info);
			free(data->info->var);
			return (1);
		}
		data->exported = data->exported->next;
	}
	data->exported = tmp;
	return (0);
}

int	export(t_data *data)
{
	int		i;

	i = 0;
	if (!data->prototype[1] || !data->prototype[1][0])
		export_print(data);
	while (data->prototype[++i] && data->prototype[i][0])
	{
		if (check_export_syntax(data, i) == ERROR)
			return (error_msg(*data, M_NOVALID, 1, data->prototype[i]));
		if (!scan_env_vars(data))
			insert_var(data, NULL);
	}
	return (EXIT_SUCCESS);
}
