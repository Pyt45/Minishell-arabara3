/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 11:15:41 by zlayine           #+#    #+#             */
/*   Updated: 2019/11/05 18:28:24 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*d;
	char	*s;

	s = (char *)src;
	d = (char *)dst;
	if (!s && !d)
		return (0);
	i = 0;
	if (dst == src)
		return (dst);
	while (i < n)
	{
		*(d + i) = *(s + i);
		i++;
	}
	return (dst);
}
