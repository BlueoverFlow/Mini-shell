/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 13:31:46 by mlabrayj          #+#    #+#             */
/*   Updated: 2021/11/12 09:55:47 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

unsigned long long int	to_ullong(const char *str, int type)
{
	int						i;
	int						sign;
	unsigned long long int	ret;

	i = 0;
	ret = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign = type;
	}
	while (str[i])
		ret = ret * 10 + str[i++] - '0';
	return (ret * sign);
}

static BOOL is_over_limit(char *arg, int *negative)
{
	int		len;
	int		j;

	j = 1;
	len = ft_strlen(arg);
	if (!ft_isdigit(arg[0]))
	{
		if (arg[0] == '+' || arg[0] == '-')
		{
			if (arg[0] == '-')
				*negative = TRUE;
			len--;
		}
		else
			return (TRUE);
	}
	while (arg[++j])
	{
		if (!ft_isdigit(arg[j]))
			return (TRUE);
	}
	if ((len > 19 || len == 0))
		return (TRUE);
	return (FALSE);
}

static BOOL	is_valid_argument(char *arg)
{
	int						j;
	BOOL					negative;
	unsigned long long int	value;
	
	negative = FALSE;
	if (is_over_limit(arg, &negative))
		return (FALSE);
	value = to_ullong(arg, 1);
	if (value > LLONG_MAX)
	{
		if (!(value - 1 == LLONG_MAX && negative))
			return (FALSE);
	}
	return (TRUE);
}

int	exit_shell(t_data data)
{
	if (data.prototype[1])
	{
		if (data.prototype[2])
			return (error_msg(data, "too many arguments\n", 255, NULL));
		else if (!is_valid_argument(data.prototype[1]))
			g_shell.exit_status =
				error_msg(data, "numeric argument required\n", 255, NULL);
		else
			g_shell.exit_status = to_ullong(data.prototype[1], -1) % 256;
	}
	ft_putstr_fd("exit\n", STDERR_FILENO);
	exit(g_shell.exit_status);
}
