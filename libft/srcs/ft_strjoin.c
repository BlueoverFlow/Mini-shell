/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mezz <ael-mezz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 10:53:26 by ael-mezz          #+#    #+#             */
/*   Updated: 2021/11/11 13:06:39 by ael-mezz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

/*
	=========== ft_strjoin ======================

	ft_strjoin and ft_strjoin_and_free always allocate the returned
	value unless both strings are NULL of course! 
*/

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*nstr;

	if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	nstr = (char *)malloc(sizeof(char) * (ft_strlen(s1)
				+ ft_strlen(s2) + 1));
	if (!nstr)
		return (NULL);
	nstr = ft_strcpy(nstr, s1);
	return (ft_strcat(nstr, s2));
}

/*
	=========== ft_strjoin_and_free ======================

	if to_free = 0 ft_strjoin_and_free acts like ft_strjoin
*/

char	*ft_strjoin_and_free(char *s1, char *s2, int to_free)
{
	char	*nstr;

	if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	nstr = (char *)malloc(sizeof(char) * (ft_strlen(s1)
				+ ft_strlen(s2) + 1));
	if (!nstr)
		return (NULL);
	nstr = ft_strcpy(nstr, s1);
	nstr = ft_strcat(nstr, s2);
	if (to_free == 1 || to_free == 3)
		free(s1);
	if (to_free == 2 || to_free == 3)
		free(s2);
	return (nstr);
}
