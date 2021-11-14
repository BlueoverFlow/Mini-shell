/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/14 17:29:41 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

#include "../../headers/minishell.h"

int	make_branch(t_data *data, char *fragment)
{
	char	*token;
	int		i;

	i = -1;
	if (!*fragment)
		return (EXIT_SUCCESS);
	data->quoting_state = UNQUOTED;
	token = ft_calloc(ft_strlen(fragment) + 1, sizeof(char));
	while (fragment[++i] && !is_redirection(data, fragment, i))
		token[i] = fragment[i];
	if (!*token)
		free(token);
	return (hundle_redirection(data, fragment, token, i));
}

static int	fill_branch(t_data *data, int i)
{
	char	*fragment;
	int		ret;

	if (!data->command && data->word)
	{
		data->command = malloc(sizeof(t_command));
		data->command->file = NULL;
		data->command->prototype = NULL;
	}
	fragment = lst_to_word(data->word);
	if (!theres_atoken(fragment))
	{
		if (!data->input[i + 1] && !data->command && data->piped_cmd)
			return (1);
		return (0);
	}
	ret = make_branch(data, fragment);
	free(fragment);
	free_list(&data->word);
	if (ret)
		free_command_struct(*data);
	return (ret);
}

static int	fill_pipeline(t_data *data, int i)
{
	if (!data->input[i + 1] && data->input[i] != ' ' && data->input[i] != '|')
		ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	if (data->input[i - 1] == '|')
		return (1);
	if (data->word || !data->command)
		if (fill_branch(data, i))
			return (1);
	if (!data->command && !data->piped_cmd)
		return (0);
	ft_dlstadd_back(&data->piped_cmd, ft_dlstnew(data->command));
	free_list(&data->word);
	return (syntax_checking(data, i));
}

static	int	build_tree(t_data *data, int i)
{
	if (data->quoting_state == UNQUOTED)
	{
		if (data->input[i + 1] && (data->input[i] == ' '))
			return (fill_branch(data, i));
		else if (data->input[i] == '|')
			return (fill_pipeline(data, i));
	}
	if (!data->input[i + 1])
		return (fill_pipeline(data, i));
	ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	return (0);
}

int	parser(t_data *data)
{
	int		i;
	int		ret;

	i = -1;
	ret = 0;
	while (data->input[++i])
	{
		define_quoting_state(data, data->input, i);
		if (build_tree(data, i))
		{
			ret = error_msg(*data, M_STXERR, 1, NULL);
			break ;
		}
	}
	if (!ret)
		ret = hundle_heredoc(data);
	if (ret)
		g_shell.exit_status = ret;
	return (ret);
}
