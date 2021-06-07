/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 08:37:34 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/07 16:03:09 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    free_list(t_list **lst)
{
	t_list  *tmp;

	if (*lst == NULL)
			return ;
	tmp = (*lst)->next;
	if ((*lst)->content)
		free((*lst)->content);
	free(*lst);
	*lst = NULL;
	free_list(&tmp);
}

t_list	*ft_lstprevious(t_list *lst)
{
	if (ft_lstsize(lst) == 1)
		return (NULL);
	while (lst->next->next)
		lst = lst->next;
	return (lst);	
}

void	ft_dlstadd_back(t_list **alst, t_list *new)
{
	t_list *tmp;

	if (alst == NULL)
		return ;
	if (*alst == NULL)
	{
		*alst = new;
		return ;
	}
	tmp = ft_lstlast(*alst);
	ft_lstlast(*alst)->next = new;
	new->previous = tmp;
}

void	print_list(t_list *lst)
{
	int i = 0;

	while (lst)
	{
		printf("-- %i --: |%s|\n", i++, (char *)lst->content);
		lst = lst->next;
	}
}

void	print_content_list(t_list *lst)
{
	t_list *sub_list;
	int i = 0;
	int j = 0;

	while (lst)
	{
		sub_list = (t_list *)(lst->content);
		while (sub_list)
		{
			printf("-- %i -- %i --: |%s|\n", j, i++, (char *)sub_list->content);
			sub_list = sub_list->next;
		}
		j++;
		i = 0;
		lst = lst->next;
	}
}

void	print_lines(t_data data)
{
	t_list *cmd;
	t_list *pipes;
	t_list *tokens;

	while (data.line)
	{
		cmd = (t_list *)(data.line->content);
		while (cmd)
		{
			pipes = (t_list *)(cmd->content);
			print_content_list(pipes);
			puts("=\n=\n=\n=\n=");
			cmd = cmd->next;
		}
		data.line = data.line->next;
	}
}