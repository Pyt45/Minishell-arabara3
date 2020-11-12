/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 13:25:42 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/12 11:59:11 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*r;
	size_t	j;
	int		i;
	size_t	slen;

	j = 0;
	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start > slen)
		return (ft_strdup(""));
	if (len > slen - start)
		len = slen - start;
	r = (char *)malloc(sizeof(char) * (len + 1));
	if (!r)
		return (0);
	i = start;
	while (j < len)
	{
		r[j] = s[start];
		start++;
		j++;
	}
	r[j] = '\0';
	return (r);
}
