/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 18:16:59 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/11/10 18:56:38 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

// int	cd(t_data *data)
// {
// 	if (data->prototype[1])
// 	{
// 		if (chdir(data->prototype[1]))
// 			return (error_msg(data, M_ARGERR, data->prototype[1]));
// 	}
// 	else
// 		chdir(getenv("HOME"));
// 	return (EXIT_SUCCESS);
// }

////


/* test new CD * buttom */
int	getsize(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

void	replace_oldpwd(char **envp, char *path, int i)
{
	int	size;

	size = getsize(envp);
	while (i < size)
	{
		if (*envp[i] == 'O' && *(envp[i] + 1) == 'L'
			&& *(envp[i] + 2) == 'D' && *(envp[i] + 3) == 'P'
			&& *(envp[i] + 4) == 'W' && *(envp[i] + 5) == 'D')
			break ;
		else
			i++;
	}
	if (i < size)
	{
		free(envp[i]);
		envp[i] = ft_strjoin("OLDPWD=", path);
	}
}

int	is_var(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] - s2[i] == 0 && s1[i] != '\0' && s2[i] != '\0')
	{
		if ((s1[i] == '=' && s2[i] == '=') || ft_strcmp(s1, s2) == 0)
			return (1);
		i++;
	}
	if ((s1[i] == '=' && s2[i] == '\0')
		|| (s1[i] == '\0' && s2[i] == '='))
		return (1);
	return (0);
}

int	var_exist(char **envp, char *argv)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (is_var(envp[i], argv))
			return (i);
		i++;
	}
	return (0);
}


void	not_valid_id(char *file, char *cmd, char *error)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, file, ft_strlen(file));
	write(2, error, ft_strlen(error));
}

int	checkdir(char *argv)
{
	int		ret;

	if (argv)
	{
		if (*argv == '\0')
			return (0);
		ret = chdir(argv);
		if (ret != 0)
		{
			not_valid_id(argv, "cd: ", ": No such file or directory\n");
			g_main.exit_status = 1;
			return (1);
		}
	}
	return (0);
}

void	cd2(t_data *data, char *path)
{
	char	*tmp;
	char	*newpath;

}

static void	execute_builtin(t_data *data, char	*cmd)
{
	char		**tmp;
	
	tmp = data->prototype;
	data->prototype = ft_split(cmd, ' ');
	free(cmd);
	builtin(data);
	free_2d(data->prototype);
	data->prototype = tmp;
}

static int	update_envs(t_data *data, char *current_dir)
{
	t_file_sys	file_sys;
	
	file_sys.home = ft_getenv(data, "HOME");
	if (!file_sys.home)
		return (ERROR);
	file_sys.home = ft_getenv(data, "HOME");
	file_sys.pwd = ft_getenv(data, "PWD");
	if (!file_sys.pwd)
		execute_builtin(data, ft_strjoin_and_free("export PWD=", file_sys.home, 2));
	file_sys.old_pwd = ft_getenv(data, "OLDPWD");
	if (file_sys.old_pwd)
	{
		execute_builtin(data, ft_strdup("unset OLDPWD"));
		execute_builtin(data, ft_strjoin_and_free("export OLDPWD=", current_dir, 2));
	}
	return (0);
}

int	cd(t_data *data, char **envp)
{
	char	*current_dir;
	char	*newpath;

	current_dir = malloc(1024);
	getcwd(current_dir, 1024);
	if (!data->prototype[1])
	{
		if (update_envs(data, current_dir) == ERROR)
			return (error_msg(data, M_HMERR, current_dir));
	}
	else
	{
		if (!checkdir(data->args[1]))
		{
			newpath = malloc(100);
			if (!getcwd(newpath, 1024))
			{
				tmp = g_main.point;
				g_main.point = ft_strjoin("/.", g_main.point);
				free(tmp);
				write(2, "cd: error retrieving current directory: ", 40);
				write(2, "getcwd: cannot access parent directories: ", 42);
				ft_putendl_fd(strerror(errno), 2);
			}
			else
			{
				free(g_main.point);
				g_main.point = ft_strdup("");
			}
			new_pwd(envp, path, newpath);
			free(newpath);
		}
		
	}
	return (EXIT_SUCCESS);
}
