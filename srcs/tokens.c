/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/16 19:48:21 by ael-mezz         ###   ########.fr       */
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

static int theres_atoken(char *fragment)
{
	int i;

	i = -1;
	while (fragment[++i])
	{
		if ((fragment[i] != ' ' && fragment[i] != '\t')
			|| ((fragment[i] == ' ' && is_backslashed(i, fragment))
			||  (fragment[i] == '\t' && is_backslashed(i, fragment))))
			return (1);
	}
	free(fragment);
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

static int *int_alloc(int i, t_data data)
{
	int *p;

	p = malloc(sizeof(int));
	if (!p)
		out(1, data);
	*p = i;
	return (p);
}

static int is_redirection(char *str, int i, int quoting_state)
{
	if (((str[i] == '>' && !is_backslashed(i, str))
		|| (str[i] == '<' && !is_backslashed(i, str))) && quoting_state == UNQUOTED)
		return (1);
	return (0);
}

static void fill_file_id(t_data *data, char **fragment)
{
	if ((*fragment)[1] && ((*fragment)[0] == '>' &&
		((*fragment)[1]) == '>' && !is_backslashed(1, *fragment)))
	{
		add_node(&data->file, build_node(NULL, int_alloc(STD_APPENDED_OUTPUT, *data)));
		*fragment += 1;
	}
	else if ((*fragment)[0] == '<' && !is_backslashed(0, *fragment))
		add_node(&data->file, build_node(NULL, int_alloc(STD_INPUT, *data)));
	else
		add_node(&data->file, build_node(NULL, int_alloc(STD_OUTPUT, *data)));
	*fragment += 1;
}

static int fill_file_path(t_data *data, char *fragment, char *token)
{
	t_list_2	*last;

	last = ft_lst2last(data->file);
	if (data->file && !last->content)
	{
		if (!token)
			return (ERROR);
		last->content = token;
		return (1);
	}
	return (0);
}

static int hundle_redirection(t_data *data, char *fragment, char *token, int i)
{
	int ret;

	if (fragment[i] && is_redirection(fragment, 0, UNQUOTED))
		fill_file_id(data, &fragment);
	else
	{
		ret = fill_file_path(data, fragment, token);
		if (ret)
		{
			if (ret == ERROR)
				return (ERROR);
		}
		else
			ft_lstadd_back(&data->prototype, ft_lstnew(token));
		fragment += i;
	}
	make_branch(data, fragment);
	return (1);
}

int make_branch(t_data *data, char *fragment)
{
	char *token;
	int i;
	int ret;

	i = -1;
	if (!*fragment)
		return (1);
	token = ft_calloc(ft_strlen(fragment) + 1, sizeof(char));
	if (!token)
		out(1, *data);
	while (fragment[++i] && !is_redirection(fragment, i, data->quoting_state))
	{
		if (QUOTED_FRAGMENT && !is_backslashed(i, data->input))
			define_quoting_state(data, data->input[i]);
		token[i] = fragment[i];
	}
	data->quoting_state = UNQUOTED;
	return (hundle_redirection(data, fragment, token, i));
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

static int syntax_checking(t_data data, int option)
{
	int l;

	l = ft_strlen(data.input) - 1;
	if (option == 0 || option == 2)
		if ((data.file && data.file->content_2 && !data.file->content)
			|| ((data.input[l] == ';' && !is_backslashed(l, data.input))
			|| (data.input[l] == '|' && !is_backslashed(l, data.input)))
			|| data.quoting_state != UNQUOTED)
			return(ERROR);
	if (option == 1 || option == 2)
		if (data.quoting_state != UNQUOTED)
			return (ERROR);
	return (1);
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
	if (syntax_checking(*data, 0) == ERROR)
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
		if (syntax_checking(*data, 2) == ERROR)
			return (ERROR);
		ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	}
	if (data->word || !data->branch)
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
	return (syntax_checking(*data, 1));
}
