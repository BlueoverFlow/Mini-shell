/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 17:20:29 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/10/16 07:35:58 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static	int	theres_atoken(char *fragment)
{
	int	i;

	i = -1;
	while (fragment[++i])
	{
		if (fragment[i] != ' ' && fragment[i] != '\t')
			return (1);
	}
	free(fragment);
	return (0);
}

static	int	*int_alloc(int i, t_data *data)
{
	int	*p;

	p = malloc(sizeof(int));
	if (!p)
		error_msg(data, "ALlocation failure!\n", NORMAL_ERR);
	*p = i;
	return (p);
}

static	int	is_redirection(char *str, int i, int quoting_state)
{
	if ((str[i] == '>' || str[i] == '<') && quoting_state == UNQUOTED)
		return (1);
	return (0);
}

static int *allocate_int(int n)
{
	int *p;

	p = malloc(sizeof(int));
	*p = n;
	return (p);
}

static	int	fill_file_id(t_data *data, char **fragment, t_list *last)
{
	if ((!data->command->file) || (data->command->file && !data->file_data))
	{
		data->file_data = malloc(sizeof(t_file_data));
		data->file_data->id = -1;
		data->file_data->path = NULL;
	}
	if (data->file_data && data->file_data->id != -1 && !data->file_data->path)
		return (ERROR);
	if ((*fragment)[0] == '>')
	{
		if ((*fragment)[1] && (*fragment)[1] == '>')
		{
			data->file_data->id = APPENDED_REDIRECTED_OUTPUT;
			*fragment += 1;
		}
		else
			data->file_data->id = REDIRECTED_OUTPUT;
	}
	else if ((*fragment)[0] == '<')
	{
		if ((*fragment)[1] && (*fragment)[1] == '<')
		{
			data->file_data->id = HEREDOC;
			*fragment += 1;
		}
		else
			data->file_data->id = REDIRECTED_INPUT;
	}
	*fragment += 1;
	ft_lstadd_back(&data->command->file, ft_lstnew(data->file_data));
	return (1);
}

static int	fill_file_path(t_data *data, char *fragment, char *token, t_list *last)
{
	if (data->command->file && data->file_data && !data->file_data->path)
	{
		if (!ft_strncmp(token, "|", 2))
			return (error_msg(data, "syntax error near unexpected token `|'\n", NORMAL_ERR));
		data->file_data->path = token;
		data->file_data = NULL;
		return (1);
	}
	return (0);
}

static	BOOL closed_quotes(char *input, int i)
{
	int j;

	j = i;
	while (input[++j])
		if (input[j] == input[i])
			return (TRUE);
	return (FALSE);
}

void	define_quoting_state(t_data *data, char *input, int i)
{
	if (data->passive)
	{
		data->quoting_state = UNQUOTED;
		data->passive = FALSE;
	}
	else if (input[i] == data->quoting_state)
		data->passive = TRUE;
	if (data->quoting_state == UNQUOTED && quoted_fragment(input[i])
		&& closed_quotes(input, i))
		data->quoting_state = input[i];
}

static int	hundle_redirection(t_data *data, char *fragment, char *token, int i)
{
	t_list	*last;
	int		ret;

	last = ft_lstlast(data->command->file);
	if (fragment[i] && is_redirection(fragment, 0, UNQUOTED))
	{
		if (fill_file_id(data, &fragment, last) == ERROR)
			return (ERROR);
	}
	else
	{
		ret = fill_file_path(data, fragment, token, last);
		if (ret == ERROR)
			return (ret);
		else if (!ret)
			ft_lstadd_back(&data->command->prototype, ft_lstnew(token));
		fragment += i;
	}
	return (make_branch(data, fragment));
}

int	make_branch(t_data *data, char *fragment)
{
	char	*token;
	int		tmp;
	int		i;
	int		ret;

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

static char	*lst_to_word(t_list *lst)
{
	int		l;
	char	*str;
	int		i;

	i = 0;
	l = ft_lstsize(lst);
	str = malloc(sizeof(*str) * (l + 1));
	while (lst)
	{
		str[i++] = *(char *)lst->content;
		lst = lst->next;
	}
	str[i] = '\0';
	return (str);
}

static int	syntax_checking(t_data *data, int i)
{
	int			l;
	t_list		*last;

	if (data->command->file)
	{
		last = ft_lstlast(data->command->file);
		data->file_data = last->content;
	}
	l = ft_strlen(data->input) - 1;
	if ((data->file_data && data->file_data->id != -1 && !data->file_data->path) || (data->input[l] == '|')
		|| (data->quoting_state != UNQUOTED && !data->input[i + 1] && !data->passive))
		l = ERROR;
	data->command = NULL;
	return (l);
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
		return ((data->input[i + 1] || data->command->prototype) ? 1 : ERROR);
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
		if (data->input[i + 1] && (data->input[i] == ' ' || data->input[i] == '\t'))
			return (fill_branch(data, i));
		else if (data->input[i] == '|')
			return (fill_pipeline(data, i));
	}
	if (!data->input[i + 1])
		return (fill_pipeline(data, i));
	ft_lstadd_back(&data->word, ft_lstnew(ft_substr(data->input, i, 1)));
	return (1);
}

static int input_stream_literal(t_data *data)
{
	static	int file_number = 0;
	t_heredoc	heredoc_;

	heredoc_.input = NULL;
	file_number++;
	heredoc_.file_name = ft_strjoin("/tmp/.heredoc_", ft_itoa(file_number));
	if ((heredoc_.fd = open(heredoc_.file_name, O_CREAT | O_RDWR | O_APPEND, 0666)) < 3)
	{
		free(heredoc_.file_name);
		return (hundle_heredoc(data));
	}
	while (1)
	{
		heredoc_.input = readline("> ");
		if (!ft_strcmp(data->file_data->path, heredoc_.input))
			break ;
		write(heredoc_.fd, heredoc_.input, ft_strlen(heredoc_.input));				
		write(heredoc_.fd, "\n", 1);
	}
	free(heredoc_.input);
	data->document = heredoc_.file_name;
	close(heredoc_.fd);
	return (1);
}

int	hundle_heredoc(t_data *data)
{
	static int	i = 1;
	t_list		*tmp;
	t_list		*tmp_2;

	tmp = data->piped_cmd;
	while (data->piped_cmd)
	{
		data->command = data->piped_cmd->content;
		tmp_2 = data->command->file;
		while (data->command->file)
		{
			data->file_data = data->command->file->content;
			if (data->file_data->id == HEREDOC)
			{
				if (!i)
					return (1);
				input_stream_literal(data);
			}
			data->command->file = data->command->file->next;
		}
		data->command->file = tmp_2;
		data->piped_cmd = data->piped_cmd->next;
	}
	data->piped_cmd = tmp;
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
