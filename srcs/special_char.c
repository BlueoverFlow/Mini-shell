/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 08:05:25 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/19 09:00:29 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_backslashed(int i, char *str)
{
	int	special;
	int	d;

	special = FALSE;
	d = i - 1;
	while (d >= 0 && str[d] == '\\')
		d--;		
	while (d++ <= i - 1)
	{
		if (str[d] == '\\' && special == FALSE)
			special = TRUE;
		else if (str[d] == '\\' && special == TRUE)
			special = FALSE;
	}
	return (special);
}