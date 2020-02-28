/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 10:32:06 by zlayine           #+#    #+#             */
/*   Updated: 2019/11/06 11:44:55 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*s_list;

	if (*alst)
	{
		s_list = *alst;
		while (s_list->next)
		{
			s_list = s_list->next;
		}
		s_list->next = new;
	}
	else
		*alst = new;
}
