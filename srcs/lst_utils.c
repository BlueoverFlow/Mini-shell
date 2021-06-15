/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 08:37:34 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/06/13 17:28:00 by ael-mezz         ###   ########.fr       */
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

int	ft_lst2size(t_list_2 *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

t_list_2	*ft_lst2last(t_list_2 *lst)
{
	int	i;
	int	j;

	if (lst == NULL)
		return (NULL);
	i = ft_lst2size(lst);
	j = 0;
	while (j < i - 1)
	{
		lst = lst->next;
		j++;
	}
	return (lst);
}

t_list_2 *build_node(void *content, void *content_2)
{
	t_list_2 *new;

	new = malloc(sizeof(t_list_2));
	if ((!new) || (!content && !content_2))
		return (NULL);
	new->content_2 = (void *)content_2;
	new->content = (void *)content;
	new->next = NULL;
	return (new);
}

void add_node(t_list_2 **alst, t_list_2 *new)
{
	if (alst == NULL)
		return ;
	if (*alst == NULL)
	{
		*alst = new;
		return ;
	}
	ft_lst2last(*alst)->next = new;
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

// void	print_lines(t_data data)
// {
// 	t_list *cmd;
// 	t_list *pipes;
// 	t_list *tokens;

// 	while (data.line)
// 	{
// 		cmd = (t_list *)(data.line->content);
// 		while (cmd)
// 		{
// 			pipes = (t_list *)(cmd->content);
// 			print_content_list(pipes);
// 			puts("=\n=\n=\n=\n=");
// 			cmd = cmd->next;
// 		}
// 		data.line = data.line->next;
// 	}
// }