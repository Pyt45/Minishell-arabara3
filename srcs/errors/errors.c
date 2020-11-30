/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:12:46 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/28 12:13:55 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		print_error(char *cmd, int err, int iscmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (err == 2 && iscmd)
		ft_putstr_fd(": command not found", 2);
	else if (err)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(err), 2);
	}
	ft_putchar_fd('\n', 2);
	return (1);
}

void	exit_error(char *str, int status, t_shell *shell)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	if (status == 127)
		ft_putstr_fd(": command not found", 2);
	if (status == 200)
		status = 127;
	ft_putchar_fd('\n', 2);
	shell->ret = status;
	exit(status);
}
