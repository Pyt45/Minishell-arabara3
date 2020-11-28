/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:13:48 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/28 18:39:52 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		pwd_builtin(t_shell *shell)
{
	char	*buff;
	int		i;

	i = ft_getenv("PWD", shell->env);
	if (i == -1)
	{
		if ((buff = getcwd(NULL, 0)) == NULL)
			print_error("pwd", errno, 0);
		else
		{
			buff = (!ft_strcmp(buff, "/")) ? ft_strjoin(buff, "/") : buff;
			ft_putstr_fd(buff, 1);
			ft_putstr_fd("\n", 1);
			ft_del(buff);
		}
	}
	else
	{
		ft_putstr_fd(shell->env[i] + 4, 1);
		ft_putstr_fd("\n", 1);
	}
	return (0);
}
