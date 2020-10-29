/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:14:56 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/29 19:15:42 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		unset_builtin(t_shell *shell, t_cmds *cmds)
{
	int	i;

	i = 1;
	while (cmds->args[i] != NULL)
	{
		shell->env = ft_unset_cmd(shell, cmds->args[i]);
		i++;
	}
	return (0);
}

char	**ft_unset_cmd(t_shell *shell, char *value)
{
	int		i;

	i = 0;
	if ((i = ft_getenv(value, shell->env)) >= 0)
		shell->env = ft_remove_from_arr(i, shell->env);
	return (shell->env);
}
