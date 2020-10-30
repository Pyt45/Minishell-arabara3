/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:14:56 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/30 09:07:40 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		unset_builtin(t_shell *shell, t_cmds *cmds)
{
	int	i;
	int err;

	i = 1;
	err = 0;
	while (cmds->args[i] != NULL)
	{
		if (ft_isdigit(cmds->args[i][0]) || ft_strcmp(cmds->args[i], " "))
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
