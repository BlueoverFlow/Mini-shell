/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 19:41:24 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/11/09 07:51:47 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	sig_handler_c(void)
{
	if (g_parent_id > 0)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		kill(-130, SIGINT);
		g_parent_id = -130;
	}
	else
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_parent_id = 1;
	}
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
		sig_handler_c();
	else if (sig == SIGQUIT)
	{
		if (g_parent_id > 0)
		{
	    	g_parent_id = 131;
			kill(-131, SIGQUIT);
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		}
		else
		{
			rl_on_new_line();
			rl_redisplay();
		}
	}
	(void)sig;
}
