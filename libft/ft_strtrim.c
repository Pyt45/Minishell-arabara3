/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 13:55:45 by zlayine           #+#    #+#             */
/*   Updated: 2019/10/23 12:03:08 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static const char	*ft_trim_start(const char *s, const char *set)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (set[j])
	{
		if (set[j] == *s)
		{
			s++;
			j = 0;
		}
		else
			j++;
	}
	return (s);
}

static size_t		ft_trim_end(const char *s, const char *set)
{
	size_t	len;
	int		j;

	j = 0;
	len = ft_strlen(s);
	while (set[j] && len)
	{
		if (set[j] == s[len - 1] && s[len - 2] && s[len - 2] != '\\')
		{
			len--;
			j = 0;
		}
		else
			j++;
	}
	return (len);
}

char				*ft_strtrim(char const *s1, char const *set)
{
	char	*r;
	char	*tmp;
	size_t	len;

	if (!s1)
		return (NULL);
	tmp = (char *)s1;
	s1 = ft_trim_start(s1, set);
	len = ft_trim_end(s1, set) + 1;
	r = malloc(sizeof(char) * len);
	if (!r)
		return (NULL);
	ft_strlcpy(r, s1, len);
	ft_del(tmp);
	r[len - 1] = '\0';
	return (r);
}
