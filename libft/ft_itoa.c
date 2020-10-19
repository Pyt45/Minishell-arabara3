/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 17:06:13 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/19 12:03:37 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check_neg(int *n, char *s, int *len)
{
	int	i;

	i = 0;
	if (*n == -2147483648)
	{
		s[i++] = '-';
		s[i++] = '2';
		*n = 147483648;
		*len = 8;
	}
	else if (*n < 0)
	{
		*n = *n * -1;
		s[i++] = '-';
		*len -= 1;
	}
	return (i);
}

char		*ft_itoa(int n)
{
	int		r;
	char	*s;
	int		i;
	int		len;
	int		p;

	i = 0;
	len = ft_nblen(n);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (len)
		len--;
	if (!s)
		return (0);
	i = ft_check_neg(&n, s, &len);
	p = ft_pow(10, len);
	while (p >= 1)
	{
		r = n / p;
		s[i] = r + 48;
		n = n - (r * p);
		p /= 10;
		i++;
	}
	s[i] = '\0';
	return (s);
}
