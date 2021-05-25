/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 15:32:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/25 15:35:02 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void expansion(t_data *data)
{
	t_list *tokens;
	
	tokens = data->tokens;
	while (tokens)
	{
		if (((char *)(tokens->content))[0] == 
	}
}