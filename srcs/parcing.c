/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/25 16:11:17 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parser(t_data *data, int i)
{

	if (!to_tokens(data))
		return ;
	expansion(data);
	print_list(data->tokens);
}