/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 11:38:01 by zlayine           #+#    #+#             */
/*   Updated: 2019/10/27 17:53:02 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	*s;
	unsigned char	*d;
	unsigned char	r;

	i = 0;
	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	r = (unsigned char)c;
	while (i < n)
	{
		d[i] = s[i];
		if (s[i] == r)
			return (d + i + 1);
		i++;
	}
	return (0);
}
