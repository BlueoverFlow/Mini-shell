/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/17 18:23:24 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int next_quote(char *str, char q1, char q2)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (i == 0)
				return (i);
			if (str[i - 1] != '\\')
				return (i);
		}
	return (ERROR);
}

static BOOL valid_quote(const char *str, char q)
{
	int i;
	int j;

	i = -1;
	j = 0;
	while (str[++i])
		if (str[i] == q && str[i - 1] != '\\')
			j++;
	if (j % 2 == 0)
		return (TRUE);
	return (FALSE); 
}

static void split_with_blanks(const char *str)
{
	// if ()
}

static void merge_tokens(t_data *data, char *blanks)
{
	// if ((data->field[0] == '\'' || data->field[0] == '\"') && ft_lstsize(data->tokens) > 0)
	// 	if (data->tokens->content[ft_strlen(data->tokens->content) - 1] != blanks[0])
	// 		data->tokens->content = ft_strjoin2(data->tokens->content, data->field);
	if (data->field[0] != blanks[0] && ft_lstsize(data->tokens) > 0)
		data->tokens->content = ft_strjoin2(data->tokens->content, data->field);
	else
		ft_lstadd_back(&data->tokens, ft_lstnew(data->field));
}

static void to_tokens(t_data *data)
{
	int i;
	char blanks[2] = {' ', '\t'};

	i = -1;
	while (data->input[++i])
	{
		data->nq = next_quote(data->input + i, '\'', '\"');
		if (!data->is_quoted && data->nq != ERROR && !valid_quote(data->input + i, data->input[i + data->nq]))
			out(0);
		if (!data->nq)
			data->is_quoted = TRUE;
		if (!data->is_quoted)
		{
			if (data->nq == ERROR)
				data->field = ft_strdup2(data->input + i);
			else
			{
				data->field = ft_substr(data->input, i, data->nq);
				data->is_quoted = TRUE;
			}
			/*
			need to world split the unquoted field
			*/
		}
		else
		{
			data->field = ft_substr(data->input, i, next_quote(data->input + i + 1, '\'', '\"') + 2);
			data->is_quoted = FALSE;
		}
		// merge_tokens(data, blanks);
		i += ft_strlen(data->field) - 1;
		printf("\n-- field  --: |%s|\n", data->field);
	}
}

void	parser(t_data *data, int i)
{
	int j;

	to_tokens(data);
	print_list(data->tokens);
}