/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrayj <mlabrayj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 18:16:59 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/11/15 16:26:57 by mlabrayj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static int	switch_dir(t_data data)
{
	char	*home_path;
	char	*new_dir;

	if (data.prototype[1])
	{
		if (!*data.prototype[1])
			return (2);
		new_dir = ft_strdup(data.prototype[1]);
	}
	else
	{
		home_path = ft_getenv(data, "HOME");
		if (!home_path)
			return (error_msg(data, M_NOHOME, 1, NULL));
		new_dir = home_path;
	}
	if (chdir(new_dir) == ERROR)
	{
		free(new_dir);
		return (error_msg(data, NULL, 1, new_dir));
	}
	free(new_dir);
	return (0);
}

int	cd(t_data *data)
{
	char	*current_dir;
	int		ret;

	current_dir = getcwd(NULL, 0);
	ret = switch_dir(*data);
	if (ret || ft_strcmp(data->prototype[0], "cd"))
	{
		chdir(current_dir);
		if (ret != 1)
			ret = EXIT_SUCCESS;
		free(current_dir);
		return (ret);
	}
	execute_edited_prototype(data, ft_strjoin_and_free
		("export OLDPWD=", current_dir, 2));
	current_dir = getcwd(NULL, 0);
	execute_edited_prototype(data, ft_strjoin_and_free
		("export PWD=", current_dir, 2));
	return (EXIT_SUCCESS);
}
