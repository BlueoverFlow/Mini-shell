/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 09:41:53 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/24 09:32:38 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void out(int code)
{
	if (code == 0)
		ft_putstr_fd("Error while parsing!\n", STDERR_FILENO);
	if (code == 1)
	{
		ft_putstr_fd("Allocation error!\n", STDERR_FILENO);
		exit(1);
	}
}
