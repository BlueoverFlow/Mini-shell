/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/25 17:43:28 by ael-mezz         ###   ########.fr       */
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
	single_quote_exp(token, data, new);
}

static void double_quote_exp(t_list *token, t_data *data)
{
	
}

static void expand_input(t_list *token, t_data *data)
{
	
}

void expansion(t_data *data)
{
	t_list *tokens;

	tokens = data->tokens;
	while (tokens)
	{
		if (((char *)(tokens->content))[0] == '\'')
			single_quote_exp(tokens, data, NULL);
		else if (((char *)(tokens->content))[0] == '"')
			double_quote_exp(tokens, data);
		else
			expand_input(tokens, data);
		tokens = tokens->next;
	}
}