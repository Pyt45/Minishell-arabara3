/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 10:31:23 by zlayine           #+#    #+#             */
/*   Updated: 2019/10/23 09:47:31 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	char			*s;
	unsigned char	r;

	i = 0;
	r = (unsigned char)c;
	s = (char *)b;
	while (i < len)
		s[i++] = r;
	return (b);
}
