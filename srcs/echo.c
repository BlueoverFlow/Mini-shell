/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 12:58:43 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/06/05 09:43:36 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	check_ifn(char **args)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (args[++i])
	{
		j = 0;
		if (args[i][j++] == '-' && args[i][j] && args[i][j] == 'n')
		{
			while (args[i][j] == 'n')
				j++;
			if (args[i][j] && args[i][j] != 'n')
				return (1);
		}
		else
			return (i);
	}
	return (i); 
}

int     echo(char **args)
{
	size_t	i;
	int		n;
	t_var	d;

	n = 1;
	if (d._error != 1)
		d._status = 0;
	if (!args[1])
	{
		ft_putchar_fd('\n', 1);
		return (1);
	}
	if (!ft_strcmp(args[1], " ") && !args[2])
		return (1);
	i = check_ifn(args);
	// if(i > 1)
	// 	n = 0;
	// else
	// 	n = 1;
	n = i > 1 ? 0 : 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		i++;
		if (args[i])
			ft_putchar_fd(' ', 1);
	}
	if (n)
		ft_putchar_fd('\n', 1);
	return (1);
}


// int main(int ac, char **av)
// {
// 	clear();
	
// 		// prompt();
// 	printf("%s\n", getenv("USER"));
// 	if (ac > 1)
// 	{
// 		/* code */
	
//     int j = 0;
//     // char *args[3] = {"echo", "hello World"};
// 	echo(av);
//     // printf("%d\n", j);
    
// 	}
// 	else if (ac == 1)
// 	{
// 		/* code */
// 		printf("\n");
// 	}
// 	printf("%ld", check_ifn(&av[1]));
	


// 	// char cwd[255];
// 	// printf("%s\n", getcwd(cwd, sizeof(cwd)));

//     return 0;
// }