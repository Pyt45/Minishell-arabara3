/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 18:47:58 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/29 19:36:30 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		env_builtin(t_cmds *cmd, char **env)
{
	if (cmd->args[1] != NULL)
	{
		print_error(cmd->args[1], 2, 0);
		return (127);
	}
	else
		ft_print_env_arr(env);
	return (0);
}

void	ft_print_env_arr(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i] != NULL)
		{
			if (ft_strchr(arr[i], '='))
			{
				ft_putstr_fd(arr[i], 1);
				ft_putstr_fd("\n", 1);
			}
			i++;
		}
	}
}
