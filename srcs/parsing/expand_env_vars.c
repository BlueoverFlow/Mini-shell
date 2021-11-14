/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 09:45:38 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/14 14:04:57 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*value_concatenation(char *new, char *s)
{
	int			i;
	char		**fragment;

	i = 0;
	fragment = ft_split(s, ' ');
	free(s);
	new = ft_strjoin_and_free(new, "\"", 1);
	if (ft_strlen(new) != 1 && *s == ' ')
		fragment[0] = ft_strjoin_and_free(" ", fragment[0], 2);
	new = ft_strjoin_and_free(new, fragment[0], 1);
	while (fragment[++i])
	{
		new = ft_strjoin_and_free(new, " ", 1);
		new = ft_strjoin_and_free(new, fragment[i], 1);
	}
	new = ft_strjoin_and_free(new, "\"", 1);
	free_2d(fragment);
	return (new);
}

static int	find_len(char *input, int i)
{
	int	len;
	int	len_2;
	int	len_3;

	len = find_char(input + i + 1, '\'');
	len_2 = find_char(input + i + 1, '"');
	if ((len_2 < len && len_2 != ERROR) || len == ERROR)
		len = len_2;
	len_3 = find_char(input + i + 1, '$');
	if ((len_3 < len && len_3 != ERROR) || len == ERROR)
		len = len_3;
	if (len == ERROR)
		len = ft_strlen(input) - i - 1;
	return (len);
}

static char	*assign_var_and_value(t_data *data, char *input, char *new, int *i)
{
	t_info	assign;
	int		len;

	len = find_len(input, *i);
	assign.var = ft_substr(input, *i + 1, len);
	if (*assign.var == '?')
	{
		assign.value = ft_itoa(g_shell.exit_status);
		(*i)++;
	}
	else
	{
		assign.value = ft_getenv(*data, assign.var);
		*i += ft_strlen(assign.var);
	}
	free(assign.var);
	if (assign.value)
	{
		if (data->quoting_state == UNQUOTED)
			new = value_concatenation(new, assign.value);
		else
			new = ft_strjoin_and_free(new, assign.value, 3);
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
				new = ft_strjoin_and_free(new, ft_substr(input, i, 1), 3);
	}
	free(input);
	return (new);
}
