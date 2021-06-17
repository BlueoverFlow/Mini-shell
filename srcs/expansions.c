/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/17 19:42:20 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_special_char(int i, char *input, char *special)
{
	int j;

	j = -1;
	while (special[++j] != '\0')
		if (!is_backslashed(i, input) && input[i] == special[j]) 
			return (1);
	return (0);
}

char	*expand_unquoted_token(t_data *data, char *input)
{
	int i;
	int j;
	char *new;
	char special[4] = {' ', '\t', '\\', '\0'};
	
	j = 0;
	i = -1;
	init_2(data);
	new = ft_calloc(ft_strlen(input) + 1, sizeof *new);
	if (!new)
		out(1, *data);
	while (input[++i])
	{
		if (quoted_fragment(input[i]) && !is_backslashed(i, input))
			define_quoting_state(data, input, i);
		if (data->quoting_state == UNQUOTED && is_special_char(i, input, special)) 
			continue ;
		new[j++] = input[i];
	}
	new[j] = '\0';
	free(input);
	return (new);
}

char	*expand_in_double_quote(t_data *data, char *input)
{
	int i;
	int j;
	char *new;
	char special[3] = {'"', '\\', '\0'};
	
	j = 0;
	i = -1;
	init_2(data);
	new = ft_calloc(ft_strlen(input) + 1, sizeof *new);
	if (!new)
		out(1, *data);
	while (input[++i])
	{
		if (quoted_fragment(input[i]) && !is_backslashed(i, input))
			define_quoting_state(data, input, i);
		if ((input[i] == '"') && ((data->quoting_state == '"'
			&& is_special_char(i, input, special)) || !input[i + 1]))
			continue ;
		new[j++] = input[i];
	}
	new[j] = '\0';
	free(input);
	return (new);
}

char *expand_in_single_quote(t_data *data, char *input)
{
	int		i;
	int		j;
	char	*new;

	j = 0;
	i = -1;
	init_2(data);
	new = ft_calloc(ft_strlen(input) + 1, sizeof *new);
	if (!new)
		out(1, *data);
	while (input[++i])
	{
		if (quoted_fragment(input[i]) && !is_backslashed(i, input))
			define_quoting_state(data, input, i);
		if ((input[i] == '\'') && (data->quoting_state == '\'' || !input[i + 1]))
			continue ;
		new[j++] = input[i];
	}
	new[j] = '\0';
	free(input);
	return (new);
}

