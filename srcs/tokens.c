/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/16 12:24:18 by ael-mezz         ###   ########.fr       */
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
	free(frg);
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

static char *lst_to_string(t_list *lst)
{
	int l;
	char *str;
	int i = 0;

	l = ft_lstsize(lst);
	str = malloc(sizeof * str * (l + 1));
	while (lst)
	{
		str[i++] = *(char *)lst->content;
		lst = lst->next;
	}
	str[i] = '\0';
	return (str);
}

static int fill_branch(t_data *data, int i)
{
	char *fragment;

	fragment = lst_to_string(data->word);
	if (!theres_atoken(fragment))
		return (ERROR);
	if (make_branch(data, fragment) == ERROR)
			return (ERROR);
	free(fragment);
	if (data->word)
		free_list(&data->word);
	return (1);
}

static int fill_pipeline(t_data *data, int i)
{
	if (data->word || !data->prototype)
		if (fill_branch(data, i) == ERROR)
			return (ERROR);
	add_node(&data->branch, build_node(data->file, data->prototype));
	data->file = NULL;
	data->prototype = NULL;
	ft_lstadd_back(&data->piped, ft_lstnew(data->branch));
	data->branch = NULL;
	free_list(&data->word);
	return (1);
}

static int fill_command(t_data *data, int i)
{
	if (!data->input[i + 1])
	{
		if ((data->input[i] == ';' && !is_backslashed(i, data->input))
			|| (data->input[i] == '|' && !is_backslashed(i, data->input)))
			return (ERROR);
		ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	}
	if (data->word || !data->prototype)
		if (fill_pipeline(data, i) == ERROR)
			return (ERROR);
	ft_lstadd_back(&data->commands, ft_lstnew(data->piped));
	data->piped = NULL;
	free_list(&data->word);
	return (1);
}

static int build_tree(t_data *data, int i)
{
	if (!is_backslashed(i, data->input) && data->quoting_state == UNQUOTED)
	{
		if (data->input[i] == ';' || !data->input[i + 1])
			return (fill_command(data, i));
		else if (data->input[i] == ' ' || data->input[i] == '\t')
		{
			fill_branch(data, i);
			return (1);
		}
		else if (data->input[i] == '|')
			return (fill_pipeline(data, i));
	}
	if (!data->input[i + 1])
		fill_command(data , i);
	ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	return (1);
}

static int syntax_checking(t_data data)
{
	if (data.quoting_state != UNQUOTED)
		return(ERROR);
	return (1);
}

int extract_branches(t_data *data)
{
	int		i;

	i = -1;
	while (data->input[++i])
	{
		if (QUOTED_FRAGMENT && !is_backslashed(i, data->input))
			define_quoting_state(data, data->input[i]);
		if (build_tree(data, i) == ERROR)
			return(ERROR);
	}
	return (syntax_checking(*data));
}