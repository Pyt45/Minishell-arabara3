/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:12:50 by zlayine           #+#    #+#             */
/*   Updated: 2019/10/31 10:38:28 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	r;
	int		len;

	r = (char)c;
	len = ft_strlen(s);
	while (len >= 0)
	{
		if (s[len] == r)
			return ((char *)s + len);
		len--;
	}
	return (NULL);
}
