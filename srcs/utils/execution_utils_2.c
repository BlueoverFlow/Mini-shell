/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 16:54:46 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/30 18:21:51 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	is_plus_sign(t_data *data, char *var, int i)
{
	if (var[i] == '+' && i == (int)ft_strlen(var) - 1 && data->info->value)
		return (1);
	return (0);
}

char	**env_array(t_data *data)
{
	char	**envp;
	int		i;

	i = 0;
	envp = malloc(sizeof(*envp) * (ft_lstsize(data->exported) + 1));
	while (data->exported)
	{
		data->info = data->exported->content;
		envp[i++] = ft_strjoin_and_free_s1
			(ft_strjoin(data->info->var, "="), data->info->value);
		data->exported = data->exported->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	error_msg(t_data *data, int errno_code, char *file)
{
	error_prompt(data, file);
	if (errno_code == M_NOCMD || errno_code == M_NOEXENT)
	{
		if (errno_code == M_NOCMD)
			ft_putstr_fd("command not found\n", STDERR_FILENO);
		else
			ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		return (127);
	}
	else if (errno_code == M_ARGERR || errno_code == M_NOVALID)
	{
		if (errno_code == M_NOVALID)
			ft_putstr_fd("not a valid identifier\n", STDERR_FILENO);
		else
			perror(NULL);
		return (1);
	}
	else if (errno_code == M_BADACCES)
	{
		ft_putstr_fd("can't access/execute\n", STDERR_FILENO);
		return (126);
	}
	ft_putstr_fd("syntax error!\n", STDERR_FILENO);
	return (258);
}
