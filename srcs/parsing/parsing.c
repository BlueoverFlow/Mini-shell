/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/20 09:27:40 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	make_branch(t_data *data, char *fragment)
{
	char	*token;
	int		tmp;
	int		i;

	i = 0;
	if (!*fragment)
		return (1);
	tmp = data->passive;
	token = ft_calloc(ft_strlen(fragment) + 1, sizeof(char));
	if (!token)
		error_msg(data, "ALlocation failure!\n", NORMAL_ERR);
	define_quoting_state(data, data->input, i--);
	while (fragment[++i] && !is_redirection(fragment, i, data->quoting_state))
		token[i] = fragment[i];
	data->passive = tmp;
	return (hundle_redirection(data, fragment, token, i));
}

static int	fill_branch(t_data *data, int i)
{
	char	*fragment;

	if (!data->command)
	{
		data->command = malloc(sizeof(t_command));
		data->command->file = NULL;
		data->command->prototype = NULL;
	}
	fragment = lst_to_word(data->word);
	if (!theres_atoken(fragment))
	{
		if (data->input[i + 1] || data->command->prototype)
			return (1);
		return (ERROR);
	}
	if (make_branch(data, fragment) == ERROR)
		return (ERROR);
	free(fragment);
	if (data->word)
		free_list(&data->word);
	return (1);
}

static int	fill_pipeline(t_data *data, int i)
{
	if (!data->input[i + 1])
		ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	if (data->input[i - 1] == '|')
		return (ERROR);
	if (data->word || !data->command)
		if (fill_branch(data, i) == ERROR)
			return (ERROR);
	ft_dlstadd_back(&data->piped_cmd, ft_dlstnew(data->command));
	free_list(&data->word);
	return (syntax_checking(data, i));
}

static	int	build_tree(t_data *data, int i)
{
	if (data->quoting_state == UNQUOTED)
	{
		if (data->input[i + 1]
			&& (data->input[i] == ' ' || data->input[i] == '\t'))
			return (fill_branch(data, i));
		else if (data->input[i] == '|')
			return (fill_pipeline(data, i));
	}
	if (!data->input[i + 1])
		return (fill_pipeline(data, i));
	ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	return (1);
}

int	parser(t_data *data)
{
	int		i;

	i = -1;
	while (data->input[++i])
	{
		define_quoting_state(data, data->input, i);
		if (build_tree(data, i) == ERROR)
			return (error_msg(data, "syntax error!\n", NORMAL_ERR));
	}
	return (hundle_heredoc(data));
}
