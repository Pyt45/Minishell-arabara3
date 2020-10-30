/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:14:56 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/30 17:43:20 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		unset_builtin(t_shell *shell, t_cmds *cmds)
{
	int	i;
	int err;

	i = 1;
	err = 0;
	while (cmds->args[i])
	{
		if (!valid_arg_name(cmds->args[i]))
			err = print_error("invalid identifier", errno, 0);
		shell->env = ft_unset_cmd(shell, cmds->args[i]);
		i++;
	}
	return (err);
}

char	**ft_unset_cmd(t_shell *shell, char *value)
{
	int		i;

	i = 0;
	if ((i = ft_getenv(value, shell->env)) >= 0)
		shell->env = ft_remove_from_arr(i, shell->env);
	return (shell->env);
}
