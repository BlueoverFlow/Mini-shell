/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/25 19:22:55 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_special_char(int i, char *input, char *special)
{
	int j;

	j = -1;
	while (special[++j] != '\0')
		if (input[i] == special[j])
			return (1);
	return (0);
}

// static int find_value(t_data *data, char *var, char **new)
// {
// 	char *tmp;
// 	char *key;
// 	char *value;
// 	int	i;
// 	int l;

// 	skip = 0;
// 	if (!data->exported)
// 		export(data, (tmp = ft_split(".", ' '), 1));
// 	while (data->exported)
// 	{
// 		i = find_char(data->exported->content + 11, '=');
// 		l = ft_strlen(data->exported->content);
// 		key = ft_substr(data->exported->content, 11, (i == ERROR ? l : i));
// 		if (!ft_strcmp(key, *var))
// 		{
// 			if (i = ERROR)
// 				return (0);
// 			value = ft_substr(input, i + 1, l);
// 			value = expand_token(data, value);
			
			
// 		}
// 	}
// }

// void	expand_env_var(t_data *data, char *input)
// {
// 	int		i;
// 	int		j;
// 	int		ret;
// 	char	*var;
// 	char	*new;

// 	i = -1;
// 	j = 0;
// 	data->quoting_state = UNQUOTED;
// 	while (input[++i])
// 	{
// 		define_quoting_state(data, input, i);
// 		if (input[i] == '$')
// 		{
// 			if (!input[i + 1])
// 			{
// 				printf("$\n");
// 				return ;
// 			}
// 			else if (ft_isalpha(input[i + 1]) && data->quoting_state != '\'')
// 			{
// 				var = ft_substr(input, i + 1, ft_strlen(input));
// 				ret = find_value(data, var, &new);
// 				if (ret != ERROR)
// 					j += ret;
// 			}
// 		}
// 		new[j++] = input[i];
// 	}
// }

char	*expand_token(t_data *data, char *input)
{
	int j;
	int i;
	char special[4] = {' ', '\t', '\0'};
	char special_2[3] = {'"', '\0'};
	char *new;
	
	j = 0;
	data->quoting_state = UNQUOTED;
	new = ft_calloc(ft_strlen(input) + 1, sizeof *new);
	if (!new)
		out(data, "ALlocation failure!\n", 0);
	i = -1;
	while (input[++i])
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
