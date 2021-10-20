/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 18:16:59 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/10/20 09:38:19 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	cd(char *prototype)
{
	// struct dirent	*de;// Pointer for directory entry
    // int				ret;
	// char			s[1000];
	// DIR				*dr;

    // // opendir() returns a pointer of DIR type. 
	// dr = opendir(".");
	// // printf("%s current\n", getcwd(s, 1000));
	// ret = chdir(prototype);
	// printf("%s changed\n", getcwd(s, 1000));
	// printf("ret = %d\n", ret);
	// if (ret == -1 )
	// {
	// 	// char *home = getenv("HOME");
	// 	chdir(getenv("HOME"));
	// }
	// if (dr == NULL) // opendir returns NULL if couldn't open directory
	// {
	// 	printf("Could not open current directory" );
	// 	return (0);
	// }
    // // for readdir()
	// // while ((de = readdir(dr)) != NULL)
	// // {
	// // 	printf("%s\n", de->d_name);
    // //     // if(ft_strcmp(de->d_name, av[1]))
    // //     //     readdir(dr);
	// // }
	// closedir(dr);

	if (prototype)
	{
		if (chdir(prototype))
			printf("%s: %s%s No such file or directory\n", "minishell: cd" ,prototype,":");
	}
	else
		chdir(getenv("HOME"));
	return (0);
}