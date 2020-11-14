/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 18:43:08 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/14 14:53:46 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		echo_print(char **str, int pos)
{
	ft_putstr_fd(str[pos], 1);
	if (str[pos + 1])
		ft_putchar_fd(' ', 1);
}

int			check_n_flag(char *str, int *n)
{
	int i;
	int	is_flag;

	i = 0;
	is_flag = 0;
	if (str[0] == '-' && str[1] == 'n' && (str[2] == 'n' || !str[2]))
	{
		*n = 2;
		is_flag = 1;
		while (str[*n] == 'n')
			*n = *n + 1;
		if (str[*n])
			is_flag = 0;
	}
	return (is_flag);
}

int			echo_builtin(t_cmds *cmd)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (!cmd->args[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	while (cmd->args[++i])
	{
		if (!n)
			while (check_n_flag(cmd->args[i], &n))
				i++;
		echo_print(cmd->args, i);
		if (!cmd->args[i + 1] && !n)
			ft_putchar_fd('\n', 1);
	}
	return (0);
}
