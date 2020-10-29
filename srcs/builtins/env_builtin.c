/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 18:47:58 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/29 18:49:19 by zlayine          ###   ########.fr       */
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
