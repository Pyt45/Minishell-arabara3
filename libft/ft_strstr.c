/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 17:26:43 by zlayine           #+#    #+#             */
/*   Updated: 2019/12/25 17:20:18 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	const char	*start;
	const char	*pattern;

	if (!*needle)
		return ((char *)haystack);
	while (*haystack)
	{
		start = haystack;
		pattern = needle;
		while (*haystack && *pattern && *haystack == *pattern)
		{
			haystack++;
			pattern++;
		}
		if (!*pattern)
			return ((char *)start);
		haystack = start + 1;
	}
	return (0);
}
