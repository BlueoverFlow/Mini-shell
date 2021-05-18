/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 08:37:34 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/18 08:23:34 by ael-mezz         ###   ########.fr       */
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