/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:20:34 by zlayine           #+#    #+#             */
/*   Updated: 2019/10/31 19:03:41 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	full;

	if (!src)
		return (0);
	full = ft_strlen(src);
	i = 0;
	if (size > 0)
	{
		while (*src && i < size - 1)
		{
			*dest = *src;
			dest++;
			src++;
			i++;
		}
		*dest = '\0';
	}
	return (full);
}
