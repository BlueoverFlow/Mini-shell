/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 16:33:50 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/06/29 12:04:46 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <signal.h>

void    my_exit(void)
{
    printf("exit\n");
    exit(1);
}

int main(void)
{
    my_exit();
    return 0;
}