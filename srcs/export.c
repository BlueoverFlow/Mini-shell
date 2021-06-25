/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 13:29:24 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/25 19:21:08 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int analyze_var(t_data *data, char *var)
{
	int i;

	i = 0;
	if (!ft_isalpha(var[0]))
		return (ERROR);
	while (var[++i])
		if (!ft_isalpha(var[i]) && (var[i] == '+' && i != ft_strlen(var) - 1))
			return (ERROR);
	return (1);
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
	if (analyze_var(data, *var) == ERROR)
		return (ERROR);
	return (1);
}

static char *quoting_var_value(t_data *data, char *line)
{
	char	*new;
	int		i;
	int		j;

	j = 0;
	i = -1;
	if (!line)
		return (NULL);
	new = ft_calloc(ft_strlen(line) + 3, sizeof *new);
	if (!new)
		out(data, "ALlocation failure!\n", 0);
	while (line[++i])
	{
		new[i + j] = line[i];
		if ((line[i] == '=' && j == 0) || (!line[i + 1] && j == 1))
		{
			j++;
			if (!line[i + 1] && j == 1)
				new[i + j++] = '"';
			new[i + j] = '"';
		}
	}
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

static void replace_value(t_data *data, char **value, int l)
{
	char	*old_value;
	int i;

	i = find_char(data->exported->content, '=');
	old_value = ft_substr(data->exported->content, i, l - i);
	old_value = remove_quotes(old_value);
	*value = ft_strjoin(old_value, *value + 1);
}

static int already_exported(t_data *data, char **var, char **value, int i)
{
	int		l;
	char	*key;
	
	i = find_char(data->exported->content + 11, '=');
	l = ft_strlen(data->exported->content);
	key = ft_substr(data->exported->content, 11, (i == ERROR ? l : i));
	if (!ft_strcmp(key, *var))
	{
		if (i != ERROR && *value && *var)
			replace_value(data, value, l);
		*var = ft_strjoin("declare -x ", *var);
		*value = quoting_var_value(data, *value);
		data->exported->content = ft_strjoin(*var, *value);
		return (1);
	}
	free(key);
	return (0);
}

static int scan_env_vars(t_data *data, char **var, char **value)
{
	t_list	*tmp;
	char	*key;
	int		i;

	tmp = data->exported;
	i = find_char(*var, '+');
	if (i != ERROR)
		(*var)[ft_strlen(*var) - 1] = '\0';
	while (data->exported)
	{
		if (already_exported(data, var, value, i))
		{
			data->exported = tmp;
			return (1);
		}
		data->exported = data->exported->next;
	}
	data->exported = tmp;
	return (0);
}

int export(t_data *data, char **prototype, int code)
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
	if (!prototype[1] && code == 0)
		print_list(data->exported);
	else if (*prototype[1] == '-')
		return (out(data, "options are unsupported!\n", 1));
	i = 0;
	while (prototype[++i])
	{
		if (check_export_syntax(data, prototype[i], &var, &value) == ERROR)
			return (out(data, ft_strjoin(var, value), 3));
		if (!scan_env_vars(data, &var, &value))
			insert_var(data, ft_strjoin(var, value));
	}
	return (1);
}
