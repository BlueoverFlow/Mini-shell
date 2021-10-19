/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 09:45:38 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/19 16:32:39 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*concate_value(t_data *data, char *new, char *s)
{
	int			i;
	char		**fragment;

	i = 0;
	fragment = ft_split(s, ' ');
	new = ft_strjoin_and_free_all(new, ft_strdup("\""));
	if (ft_strlen(new) != 1 && *s == ' ')
		fragment[0] = ft_strjoin_and_free_all(ft_strdup(" "), fragment[0]);
	new = ft_strjoin_and_free_all(new, ft_strdup(fragment[0]));
	while (fragment[++i])
	{
		new = ft_strjoin_and_free_s1(new, " ");
		new = ft_strjoin_and_free_all(new, ft_strdup(fragment[i]));
	}
	new = ft_strjoin_and_free_s1(new, "\"");
	free_2d(fragment);
	return (new);
}

static int	find_len(t_data *data, char *input, int i)
{
	int	len;
	int	tmp_len;

	len = find_char(input + i + 1, data->quoting_state);
	tmp_len = find_char(input + i + 1, '$');
	if (len == ERROR)
	{
		len = tmp_len;
		if (len == ERROR)
			len = ft_strlen(input) - i - 1;
	}
	else if (tmp_len != ERROR)
	{
		if (tmp_len < len)
			len = tmp_len;
	}
	return (len);
}

static char	*assign_var_and_value(t_data *data, char *input, char *new, int *i)
{
	t_info	assign;
	int		len;

	len = find_len(data, input, *i);
	assign.var = ft_substr(input, *i + 1, len);
	if (!ft_strcmp(assign.var, "?"))
		assign.value = ft_itoa(data->exit_status);
	else
		assign.value = ft_getenv(data, assign.var);
	*i += ft_strlen(assign.var);
	free(assign.var);
	if (assign.value)
	{
		if (data->quoting_state == UNQUOTED)
			new = concate_value(data, new, assign.value);
		else
			new = ft_strjoin_and_free_s1(new, assign.value);
	}
	return (new);
}

static BOOL	is_env_var(t_data *data, char *input, char **new, int *i)
{
	if (data->quoting_state != '\'' && input[*i + 1]
		&& input[*i] == '$' && (ft_isalnum(input[*i + 1])
			|| input[*i + 1] == '?'))
	{
		if (!ft_isdigit(input[*i + 1]))
		{
			*new = assign_var_and_value(data, input, *new, i);
			return (TRUE);
		}
		(*i)++;
		return (TRUE);
	}
	return (FALSE);
}

char	*expand_env_vars(t_data *data, char *input)
{
	int		i;
	char	*new;

	i = -1;
	new = NULL;
	data->quoting_state = UNQUOTED;
	while (input[++i])
	{
		define_quoting_state(data, input, i);
		if ((input[i] == '\"' && data->quoting_state == '\"')
			|| !is_env_var(data, input, &new, &i))
			new = ft_strjoin(new, ft_substr(input, i, 1));
	}
	free(input);
	return (new);
}
