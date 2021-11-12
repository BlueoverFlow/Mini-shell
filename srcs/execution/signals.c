/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 19:41:24 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/11/11 16:23:42 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	sig_handler_c(void)
{
	if (g_shell.parent)
	{
		g_shell.exit_status = 1;
		rl_on_new_line();
		rl_redisplay();
		write(1, "  \b\b\n", 5);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		g_shell.exit_status = 130;
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
		sig_handler_c();
	else if (sig == SIGQUIT)
	{
		if (!g_shell.parent)
		{
			write(1, "Quit: 3", 7);
			g_shell.exit_status = 131;
		}
		else
		{
			rl_on_new_line();
			rl_redisplay();
			write(1, "  \b\b", 4);
		}
	}
	if (!g_shell.parent)
		write(1, "\n", 1);
	g_shell.parent = TRUE;
}
