/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 09:41:53 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/25 19:10:02 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void out(int code, t_data data)
{
	if (code == 0)
		ft_putstr_fd("Error while parsing!\n", STDERR_FILENO);
	if (code == 1)
	{
		free_list(&data.tokens);
		free_list(&data.garbage);
		ft_putstr_fd("Allocation error!\n", STDERR_FILENO);
		exit(1);
	}
}
