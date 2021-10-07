/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 18:16:59 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/10/06 09:31:49 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cd(t_data *data, char *prototype)
{
	struct dirent	*de;// Pointer for directory entry
    int				ret;
	char			s[1000];
	DIR				*dr;

	printf("hello cd\n");
    // opendir() returns a pointer of DIR type. 
	dr = opendir(".");
	// printf("%s current\n", getcwd(s, 1000));
	ret = chdir(prototype);
	// printf("%s changed\n", getcwd(s, 1000));
	// printf("ret = %d\n", ret);
	if (dr == NULL) // opendir returns NULL if couldn't open directory
	{
		printf("Could not open current directory" );
		return (0);
	}
    // for readdir()
	// while ((de = readdir(dr)) != NULL)
	// {
	// 	printf("%s\n", de->d_name);
    //     // if(ft_strcmp(de->d_name, av[1]))
    //     //     readdir(dr);
	// }
	closedir(dr);
	return (0);
}