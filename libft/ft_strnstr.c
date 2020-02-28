/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:26:43 by zlayine           #+#    #+#             */
/*   Updated: 2019/10/23 09:55:19 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		i;
	const char	*start;
	const char	*pattern;

	i = 0;
	if (!*needle)
		return ((char *)haystack);
	while (*haystack)
	{
		start = haystack;
		pattern = needle;
		while (*haystack && *pattern && *haystack == *pattern && i < len)
		{
			haystack++;
			pattern++;
			i++;
		}
		if (!*pattern)
			return ((char *)start);
		haystack = start + 1;
		if (*pattern == *needle)
			i++;
	}
	return (0);
}
