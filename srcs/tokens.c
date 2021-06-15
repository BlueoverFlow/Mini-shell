/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/13 18:25:16 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void define_quoting_state(t_data *data, char c)
{
	int state;

	if (c == '\'')
		state = SNG_QUT;
	else if (c == '"')
		state = DBL_QUT;
	if (data->quoting_state == UNQUOTED)
		data->quoting_state = state;
	else if (data->quoting_state == state)
		data->quoting_state = UNQUOTED;
}

static int theres_atoken(char *frg)
{
	int i;

	i = -1;
	while (frg[++i])
	{
		if ((frg[i] != ' ' && frg[i] != '\t')
			|| ((frg[i] == ' ' && is_backslashed(i, frg))
			||  (frg[i] == '\t' && is_backslashed(i, frg))))
			return (1);
	}
	return (0);
}

static int find_chars(char *str, char* chars)
{
	int i;
	int j;

	i = -1;
	if (str)
		while (str[++i])
		{
			j = -1;
			if (chars)
				while (chars[++j])
					if (str[i] == chars[j] && !is_backslashed(i, str))
						return (i);
		}
	return (i);
}

static int *int_alloc(int i)
{
	int *p;

	p = malloc(sizeof(int));
	*p = i;
	return (p);
}

static int is_redirection(char *str, int i)
{
	if ((str[i] == '>' && !is_backslashed(i, str))
		|| (str[i] == '<' && !is_backslashed(i, str)))
		return (1);
	return (0);
}

static int make_branch(t_data *data, char *frg)
{
	char *token;
	int i;
	int start;
	int	end;
	t_list_2 *last;

	last = ft_lst2last(data->file);
	token = ft_calloc(ft_strlen(frg) + 1, sizeof(char));
	if (!token)
	{
		out(1, *data);
		return (ERROR);
	}
	i = -1;
	while (frg[++i] && !is_redirection(frg, i))
		token[i] = frg[i];
	if (data->file && !last->content)
	{
		if (!token)
			return (ERROR);
		last->content = token;
	}
	else
		ft_lstadd_back(&data->prototype, ft_lstnew(token));
	if (frg[i])
	{
		if (frg[i + 1] && (frg[i] == '>'
			&& (frg[i + 1] == '>' && !is_backslashed(i + 1, frg))))
			{
				add_node(&data->file, build_node(NULL, int_alloc(STD_APPENDED_OUTPUT)));
				i++;
			}
		else if (frg[i] == '<' && !is_backslashed(i, frg))
			add_node(&data->file, build_node(NULL, int_alloc(STD_INPUT)));
		else
			add_node(&data->file, build_node(NULL, int_alloc(STD_OUTPUT)));
		make_branch(data, frg + i);
	}
	return (1);
}

static int fill_branch(t_data *data, int i)
{
	char	*fragment;

	fragment = ft_substr(data->input, data->skip, i - data->skip);
	if (!theres_atoken(fragment))
		return (ERROR);
	data->skip = i + 1;
	if (make_branch(data, fragment) == ERROR)
			return (ERROR);
	return (1);
}

static int fill_pipeline(t_data *data, int i)
{
	if (!data->prototype)
		if (fill_branch(data, i) == ERROR)
			return (ERROR);
	add_node(&data->branch, build_node(data->file, data->prototype));
	data->file = NULL;
	data->prototype = NULL;
	ft_lstadd_back(&data->piped, ft_lstnew(data->branch));
	data->branch = NULL;
	return (1);
}

static int fill_command(t_data *data, int i)
{
	if (!data->piped)
		if (fill_pipeline(data, i) == ERROR)
			return (ERROR);
	ft_lstadd_back(&data->commands, ft_lstnew(data->piped));
	data->piped = NULL;
	return (1);
}

static int build_tree(t_data *data, int i)
{
	if (!is_backslashed(i, data->input))
	{
		if (data->input[i] == ' ' || data->input[i] == '\t')
			fill_branch(data, i);
		else if (data->input[i] == '|')
			return (fill_pipeline(data, i));
		else if (data->input[i] == ';' || !data->input[i + 1])
			return (fill_command(data, i));
	}
	return (1);
}

int extract_branches(t_data *data)
{
	int		i;
	char c;

	i = -1;
	data->skip = 0;
	data->quoting_state = UNQUOTED;
	while (data->input[++i])
	{
		c = data->input[i];
		printf ("c: %c\n", c);
		if (QUOTED_FRAGMENT && !is_backslashed(i, data->input))
			define_quoting_state(data, data->input[i]);
		if (data->quoting_state == UNQUOTED)
			if (build_tree(data, i) == ERROR)
				return (ERROR);
	}
	free(data->input);
	return (1);
}