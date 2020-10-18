/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 14:41:54 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/18 17:44:28 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		count_strs(char *str, char c)
{
	int i;
	int j;
	int start;

	i = 0;
	j = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			if (start == 1)
			{
				j++;
				start = 0;
			}
		}
		else if (str[i] != c)
			start = 1;
		i++;
	}
	if (start)
		j++;
	return (j);
}

char			**free_list(char **split)
{
	int i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
	return (NULL);
}

static char		**ft_make_index(char **split, char *str, char x)
{
	int i;
	int j;
	int c;

	i = -1;
	j = -1;
	c = 0;
	while (str[++i])
	{
		if (str[i] == x && j != -1)
		{
			if (!(split[c++] = ft_substr(str, j, i - j)))
				return (free_list(split));
			j = -1;
		}
		else if (str[i] != x)
		{
			if (j == -1)
				j = i;
		}
	}
	if (j != -1)
		if (!(split[c] = ft_substr(str, j, i - j)))
			return (free_list(split));
	return (split);
}

char			**ft_split(char const *s, char c)
{
	char	**r;
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	len = count_strs((char *)s, c);
	r = (char **)malloc(sizeof(char *) * (len + 1));
	if (!r)
		return (0);
	r = ft_make_index(r, (char *)s, c);
	if (!r)
		return (NULL);
	r[len] = 0;
	return (r);
}
