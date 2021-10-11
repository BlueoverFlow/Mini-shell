/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/11 11:41:09 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_special_char(int i, char *input, char *special)
{
	int	j;

	j = -1;
	while (special[++j] != '\0')
		if (input[i] == special[j])
			return (1);
	return (0);
}

static char	*assign_value(t_data *data, char *var)
{
	char	*value;
	t_list	*tmp;

	if (!data->exported)
		build_env_vars(data);
	tmp = data->exported;
	value = NULL;
	while (data->exported)
	{
		data->info = data->exported->content;
		if (!ft_strcmp(data->info->var, var))
		{
			value = ft_strdup(data->info->var);
			break ;
		}
		data->exported = data->exported->next;
	}
	data->exported = tmp;
	value = expand_token(data, value);
	return (value);
}

char	*expand_env_vars(t_data *data, char *input)
{
	int		i;
	int		len;
	char	*var;
	char	*new;

	i = -1;
	new = NULL;
	data->quoting_state = UNQUOTED;
	while (input[++i])
	{
		define_quoting_state(data, input, i);
		if (data->quoting_state != '\'' && input[i + 1]
			&& input[i] == '$' && ft_isalpha(input[i + 1]))
		{
			if ((len = find_char(input + i + 1, '$')) == ERROR)
				len = ft_strlen(input) - i - 1;
			var = ft_substr(input, i + 1, len);
			new = ft_strjoin(new, assign_value(data, var) + 1);
			i += ft_strlen(var);
		}
		else
			new = ft_strjoin(new, ft_substr(input, i, 1));
	}
	free (input);
	return (new);
}

char	*expand_token(t_data *data, char *input)
{
	int		j;
	int		i;
	char	special[4] = {' ', '\t', '\0'};
	char	special_2[3] = {'"', '\0'};
	char	*new;

	if (!input)
		return (NULL);
	j = 0;
	data->quoting_state = UNQUOTED;
	new = ft_calloc(ft_strlen(input) + 1, sizeof(*new));
	if (!new)
		error_msg(data, "ALlocation failure!\n", NORMAL_ERR);
	i = -1;
	while (input && input[++i])
	{
		define_quoting_state(data, input, i);
		if ((data->quoting_state == UNQUOTED && is_special_char(i, input, special))
			|| (data->quoting_state == '"' && is_special_char(i, input, special_2))
			|| (data->quoting_state == '\'' && input[i] == '\''))
			continue ;
		new[j++] = input[i];
	}
	free(input);
	return (new);
}
