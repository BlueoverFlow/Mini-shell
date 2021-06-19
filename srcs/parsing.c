/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@sudent.1337.ma>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/19 15:38:14 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if ((str[i] == '>' || str[i] == '<') && quoting_state == UNQUOTED)
		return (1);
	return (0);
}

static int fill_file_id(t_data *data, char **fragment, t_list_2 *last)
{
	if (data->file && !last->content)
		return (ERROR);
	if ((*fragment)[0] == '>')
	{
		if ((*fragment)[1] && (*fragment)[1] == '>')
		{
			add_node(&data->file, build_node(NULL, int_alloc(STD_APPENDED_OUTPUT, *data)));
			*fragment += 1;
		}
		else
			add_node(&data->file, build_node(NULL, int_alloc(STD_OUTPUT, *data)));
	}
	else if ((*fragment)[0] == '<')
	{
		if ((*fragment)[1] && (*fragment)[1] == '<')
		{
			add_node(&data->file, build_node(NULL, int_alloc(STD_APPENDED_INPUT, *data)));
			*fragment += 1;
		}
		else
			add_node(&data->file, build_node(NULL, int_alloc(STD_INPUT, *data)));
	}
	*fragment += 1;
	return (1);
}

static int fill_file_path(t_data *data, char *fragment, char *token, t_list_2 *last)
{
	if (data->file && !last->content)
	{
		last->content = token;
		return (1);
	}
	return (0);
}

void define_quoting_state(t_data *data, char *input, int i)
{
	if (data->passive)
	{
		data->quoting_state = UNQUOTED;
		data->passive = FALSE;
	}
	else if (input[i] == data->quoting_state)
			data->passive = TRUE;
	if (data->quoting_state == UNQUOTED && quoted_fragment(input[i]))
			data->quoting_state = input[i];
}

static int hundle_redirection(t_data *data, char *fragment, char *token, int i)
{
	t_list_2	*last;

	last = ft_lst2last(data->file);
	if (fragment[i] && is_redirection(fragment, 0, UNQUOTED))
	{
		if (fill_file_id(data, &fragment, last) == ERROR)
			return (ERROR);
	}
	else
	{
		if (!fill_file_path(data, fragment, token, last))
			ft_lstadd_back(&data->prototype, ft_lstnew(token));
		fragment += i;
	}
	return (make_branch(data, fragment));
}

int make_branch(t_data *data, char *fragment)
{
	char *token;
	int tmp;
	int i;
	int ret;

	i = 0;
	if (!*fragment)
		return (1);
	tmp = data->passive;
	token = ft_calloc(ft_strlen(fragment) + 1, sizeof(char));
	if (!token)
		out(1, *data);
	define_quoting_state(data, data->input, i--);
	while (fragment[++i] && !is_redirection(fragment, i, data->quoting_state))
		token[i] = fragment[i];
	data->passive = tmp;
	return (hundle_redirection(data, fragment, token, i));
}

static char *lst_to_word(t_list *lst)
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

static int syntax_checking(t_data data, int i)
{
	int l;
	t_list_2 *last;

	last = ft_lst2last(data.file);
	l = ft_strlen(data.input) - 1;
	if ((last && last->content_2 && !last->content)
		|| (data.input[l] == '|')
		|| (data.quoting_state != UNQUOTED && !data.input[i + 1] && !data.passive))
		l = ERROR;
	data.file = NULL;
	data.prototype = NULL;
	data.branch = NULL;
	return(l);
}

static int fill_branch(t_data *data, int i)
{
	char *fragment;

	fragment = lst_to_word(data->word);
	if (!theres_atoken(fragment))
		return((data->input[i + 1]) ? ERROR : 1);
	if (make_branch(data, fragment) == ERROR)
			return (ERROR);
	free(fragment);
	if (data->word)
		free_list(&data->word);
	return (1);
}

static int fill_pipeline(t_data *data, int i)
{
	if (!data->input[i + 1])
		ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	if (data->word || !data->prototype)
		if (fill_branch(data, i) == ERROR)
			return (ERROR);
	add_node(&data->branch, build_node(data->file, data->prototype));
	ft_lstadd_back(&data->piped, ft_lstnew(data->branch));
	free_list(&data->word);
	return (syntax_checking(*data, i));
}

static int build_tree(t_data *data, int i)
{
	if (data->quoting_state == UNQUOTED)
	{
		if (data->input[i] == ' ' || data->input[i] == '\t')
			return (fill_branch(data, i));
		else if (data->input[i] == '|')
			return (fill_pipeline(data, i));
	}
	if (!data->input[i + 1])
		return (fill_pipeline(data, i));
	ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	return (1);
}

static int extract_branches(t_data *data)
{
	int		i;

	i = -1;
	while (data->input[++i])
	{
		define_quoting_state(data, data->input, i);
		if (build_tree(data, i) == ERROR)
			return(ERROR);
	}
	return (1);
}

int	parser(t_data *data)
{
	if (extract_branches(data) == ERROR)
		return (out(0, *data));
	return (1);
}