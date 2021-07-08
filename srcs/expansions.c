/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/07/03 13:26:13 by mlabrayj         ###   ########.fr       */
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
		export(data, NULL);
	tmp = data->exported;
	value = NULL;
	while (data->exported)
	{
		if (find_value(data, var, &value))
			break ;
		data->exported = data->exported->next;
	}
	data->exported = tmp;
	value = expand_token(data, value);
	return (value);
}

char	*expand_env_var(t_data *data, char *input)
{
	int		i;
	int		len;
	char	*var;
	char	*new;

	i = -1;
	new = ft_calloc(1, sizeof(*new));
	if (!new)
		out(data, "ALlocation failure!\n", 0);
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

	j = 0;
	data->quoting_state = UNQUOTED;
	new = ft_calloc(ft_strlen(input) + 1, sizeof(*new));
	if (!new)
		out(data, "ALlocation failure!\n", 0);
	i = -1;
	while (input && input[++i])
	{
		define_quoting_state(data, input, i);
		if ((data->quoting_state == UNQUOTED && is_special_char(i, input, special)) \
			|| (data->quoting_state == '"' && is_special_char(i, input, special_2)) \
			|| (data->quoting_state == '\'' && input[i] == '\''))
			continue ;
		new[j++] = input[i];
	}
	free(input);
	return (new);
}
