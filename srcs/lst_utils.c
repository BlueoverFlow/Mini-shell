/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 08:37:34 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/05/14 09:38:00 by ael-mezz         ###   ########.fr       */
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
		{
	printf("what to free %p\n",  (*lst)->content);
			free((*lst)->content);
			(*lst)->content = NULL;	
	printf("freed %p\n",  (*lst)->content);

		}
        free(*lst);
        *lst = NULL;
        free_list(&tmp);
}