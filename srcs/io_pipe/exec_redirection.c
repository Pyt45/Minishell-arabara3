/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 11:03:45 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/07 11:36:07 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		redirect_forward(t_cmds *tmp, t_cmds *cmd)
{
	int		i;

	if (ft_arr_len(tmp->next->args) > 1 && !tmp->next->start)
	{
		i = 1;
		while (tmp->next->args[i])
		{
			cmd->args = ft_get_arr(tmp->next->args[i], cmd->args);
			i++;
		}
	}
	return open_output(tmp, tmp->append - 1);
}

int		redirect_backward(t_cmds *tmp)
{
	int		i;
	char	*file;

	i = -1;
	while (tmp->next->args[++i])
		file = tmp->next->args[i];
	return open_input(file);
}

void	exec_io_redi(t_shell *shell, t_cmds *cmd)
{
	t_cmds	*tmp;

	tmp = cmd;
	while (tmp && tmp->append)
	{
		if (tmp->append > 0)
			shell->exec.fdout = redirect_forward(tmp, cmd);
		else
		{
			shell->exec.fdout = dup(shell->exec.tmpout);
			shell->exec.fdin = redirect_backward(tmp);
			dup2(shell->exec.fdin, 0);
			close(shell->exec.fdin);
		}
		tmp = tmp->next;
	}
}
