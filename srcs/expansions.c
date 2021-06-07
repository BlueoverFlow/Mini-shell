/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/05 11:13:47 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void single_quote_exp(t_list *token, t_data *data, char *new)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (((char *)(token->content))[++i])
		if (((char *)(token->content))[i] != '\'')
		{
			if (new)
				new[j] = ((char *)(token->content))[i];
			j++;
		}
	if (new)
	{
		new[j] = '\0';
		free(token->content);
		token->content = new;
		return ;
	}
	new = malloc(sizeof(char) * (j + 1));
	if (!new)
		out(1, *data);
	single_quote_exp(token, data, new);
}

/*
still need to expand environment
*/
static void double_quote_exp(t_list *token, t_data *data, char *new)
{
	int		i;
	int		j;
	char	special[3] = {'\\', '"', '$'};

	i = 0;
	j = 0;
	while (((char *)(token->content))[++i])
	{
		if (((char *)(token->content))[i + 1]
			&& is_backslashed(i + 1, (char *)(token->content))
			&& (((char *)(token->content))[i] == special[0]
				|| ((char *)(token->content))[i] == special[1]))
			continue ;
		if (new)
			new[j] = ((char *)(token->content))[i];
		j++;
	}
	if (new)
	{
		new[--j] = '\0';
		free(token->content);
		token->content = new;
		return ;
	}
	new = malloc(sizeof(char) * j);
	if (!new)
		out(1, *data);
	double_quote_exp(token, data, new);
}

static void expand_input(t_list *token, t_data *data)
{
	/* code */
}

void analyse_tokens(t_data *data)
{
	t_list *tokens;
	int		*i;

	tokens = data->tokens;
	while (tokens)
	{
		if (((char *)(tokens->content))[0] == '\'')
			single_quote_exp(tokens, data, NULL);
		else if (((char *)(tokens->content))[0] == '"')
			double_quote_exp(tokens, data, NULL);
		else
			expand_input(tokens, data);
		tokens = tokens->next;
	}
}