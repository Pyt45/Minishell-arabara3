/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex_digit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 20:35:28 by zlayine           #+#    #+#             */
/*   Updated: 2019/11/18 20:35:40 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	hex_digit(unsigned long long v, int maj)
{
	if (v < 10)
		return ('0' + v);
	else
	{
		if (maj)
			return ('A' + v - 10);
		else
			return ('a' + v - 10);
	}
}
