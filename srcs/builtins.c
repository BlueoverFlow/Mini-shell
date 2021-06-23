/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 14:23:07 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/23 20:13:28 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int env(t_data *data, char **prototype)
{
	t_list	*tmp;
	int i;
	int j;

	i = -1;
	tmp = data->exported;
	if (prototype[1])
		return (out(data, "unsupported syntax!\n", 2));
	while (data->envp[++i])
		if (!data->exported)
			printf("%s\n", data->envp[i]);
	if (!data->exported)
		return (1);
	i--;
	j = 0;
	while (j++ < i / 2)
		tmp = tmp->next;
	while (tmp)
	{
		printf("%s\n", tmp->content + 11);
		tmp = tmp->previous;
	}
	tmp = ft_lstlast(data->exported);
	while (i-- > j)
	{
		printf("%s\n", tmp->content + 11);
		tmp = tmp->previous;
	}
	return (1);
}

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

static int     echo(char **args)
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

static char *quoting_var_value(t_data *data, char *line)
{
	char	*new;
	int		i;
	int		j;

	j = 0;
	i = -1;
	new = malloc(sizeof *new * (ft_strlen(line) + 3));
	if (!new)
		out(data, "ALlocation failure!\n", 1);
	while (line[++i])
	{
		new[i + j] = line[i];
		if (line[i] == '=' || (!line[i + 1] && j == 1))
		{
			j++;
			if (!line[i + 1] && j == 1)
				new[i + j++] = '"';
			new[i + j] = '"';
		}
	}
	new[i + j] = '\0';
	free(line);
	return (new);
}

static void insert_var(t_data *data, char *var)
{
	t_list	*last;
	char	*new;
	
	new = ft_strjoin("declare -x ", ft_strdup(var));
	new = quoting_var_value(data, new);
	ft_dlstadd_back(&data->exported, ft_dlstnew(new));
	last = ft_lstlast(data->exported);
	while (last->previous && ft_strcmp((char *)last->content + 11, (char *)last->previous->content + 11) < 0)
	{
		if (last->previous->previous)
			last->previous->previous->next = last;
		last->previous->next = last->next;
		last->next = last->previous;
		last->previous = last->next->previous;
		if (last->next->next)
			last->next->next->previous = last->next;
		last->next->previous = last;
	}
	if (!last->previous)
		data->exported = last;
}

static char *remove_quotes(char *str)
{
	char	*new;
	int		i;
	int		j;
	
	new = ft_calloc(ft_strlen(str) + 1, sizeof *new);
	if (!new)
		return(NULL);
	i = -1;
	j = 0;
	while (str[++i])
		if (!quoted_fragment(str[i]))
			new[j++] = str[i];
	return (new);
}

static char *extract_value(char *prototype, int i)
{
	char *value;

	if (!prototype[i])
		return (NULL);
	value = ft_substr(prototype, i, ft_strlen(prototype) - i);
	value = remove_quotes(value);
	//expand env
	return (value);
}

static int check_export_syntax(t_data *data, char *prototype, char **var, char **value)
{
	int i;
	int j;

	i = 0;
	while (prototype[i] && prototype[i] != '=')
		i++;
	*var = ft_substr(prototype, 0, i);
	j = -1;
	*value = extract_value(prototype, i);
	if (i == 0)
	{
		*var = *value;
		*value = NULL;
		return (ERROR);
	}
	else if (!*value)
		return (1);
	*var = expand_token(data, *var);
	while ((*var)[++j])
		if ((*var)[j] == ' ' || (*var)[j] == '/' || (*var)[j] == '\t')
			return (ERROR);
	return (1);
}

static int export(t_data *data, char **prototype)
{
	static int i = -1;
	char *var;
	char *value;
	
	if (i == -1)
	{
		data->exported = NULL;
		while (data->envp[++i])
			insert_var(data, (char *)data->envp[i]);
	}
	if (!prototype[1])
		print_list(data->exported);
	else if (*prototype[1] == '-')
		return (out(data, "options are unsupported!\n", 2));
	i = 0;
	while (prototype[++i])
	{
		if (check_export_syntax(data, prototype[i], &var, &value) == ERROR)
			return (out(data, ft_strjoin(var, NULL), 3));
		insert_var(data, ft_strjoin(var, value));
	}
	return (1);	
}

int	is_builtin(t_data *data, char **prototype)
{
	if (!ft_strcmp(prototype[0], "echo"))
		echo(prototype);
	else if (!ft_strcmp(prototype[0], "export"))
		export(data, prototype);
	else if (!ft_strcmp(prototype[0], "env"))
		env(data, prototype);
	return ((data->exit_status != 0 ) ? ERROR : 1);
}