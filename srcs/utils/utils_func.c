/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 12:23:17 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/12/07 12:35:05 by aaqlzim          ###   ########.fr       */
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

int		ft_nbr_len(long n)
{
	int i;

	i = 1;
	while (n / 10)
	{
		n /= 10;
		i++;
	}
	return (i);
}

int		check_exit_ret(char *tmp, long st)
{
	int		sign;

	sign = 0;
	if (tmp[0] == '+' || tmp[0] == '-')
		sign = 1;
	if (ft_nbr_len(st) + sign != (int)ft_strlen(tmp))
		return (0);
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
	return (ft_check_long(n, sign));
}

int		valid_status(char *arg)
{
	int i;

	i = 0;
	i = (arg[0] == '-' || arg[0] == '+') - 1;
	while (arg[++i] && ft_isdigit(arg[i]))
		;
	return (arg[i] == '\0');
}
