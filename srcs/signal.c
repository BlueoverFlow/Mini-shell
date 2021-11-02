#include "../headers/minishell.h"

static void	sig_handler_c(void)
{
	if (!""mainforkat"")
	{
		//hrdc = 1;
		//exit_stat = 1;
		rl_on_new_line();
		rl_redisplay();
		write(1, "  \b\b\n", 5);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		//exit_stat = 130;
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
		sig_handler_c();
	else if (sig == SIGQUIT)
	{
		if (""main.forkat"")
		{
			write(1, "Quit: 3", 7);
	    	//exit_stat = 131;
		}
		else
		{
			rl_on_new_line();
			rl_redisplay();
			write(1, "  \b\b", 4);
		}
	}
	if ("g_main.forkat")
		write(1, "\n", 1);
}