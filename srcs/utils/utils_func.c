/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:23:17 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/12/07 11:47:16 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		check_len(t_cmds *cmds)
{
	if (cmds && cmds->args)
	{
		if (ft_arr_len(cmds->args) > 2)
		{
			ft_putstr_fd("exit\n", 2);
			print_error("exit", 7, 0);
			return (0);
		}
	}
	return (1);
}

long	ft_atoi_l(const char *str)
{
	int			i;
	long long	n;
	int			sign;

	i = 0;
	n = 0;
	sign = 1;
	while (str[i] == '\t' || str[i] == ' ' || str[i] == '\n' ||
		str[i] == '\v' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - 48);
		i++;
	}
	n = n * sign;
	return (n);
}

int		valid_status(char *arg, long l)
{
	int i;

	if (l > INT32_MAX || l < INT32_MIN)
		return (0);
	i = 0;
	i = (arg[0] == '-' || arg[0] == '+') - 1;
	while (arg[++i] && ft_isdigit(arg[i]))
		;
	return (arg[i] == '\0');
}
