/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 11:15:58 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/18 09:32:50 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		get_num_pipes(t_cmds *cmds)
{
	int		i;
	t_cmds	*tmp;

	i = 0;
	tmp = cmds;
	while (tmp->p || tmp->append)
	{
		if (!tmp->append)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

int		is_builtin(char *cmd)
{
	if (cmd && ft_strcmp(cmd, "env") && ft_strcmp(cmd, "cd") &&
		ft_strcmp(cmd, "pwd") && ft_strcmp(cmd, "exit") &&
		ft_strcmp(cmd, "export") && ft_strcmp(cmd, "unset") &&
		ft_strcmp(cmd, "echo"))
		return (0);
	return (1);
}

void	close_pipes(int *fds, int num_pipe)
{
	int		i;

	i = 0;
	while (i < 2 * num_pipe)
		close(fds[i++]);
}

int		wait_child(t_shell *shell, pid_t pid)
{
	int		i;
	int		status;

	i = -1;
	if (!shell->num_pipe)
		waitpid(pid, &status, 0);
	else
	{
		while (++i < shell->num_pipe + 1)
		{
			waitpid(shell->exec.pids[i], &status, 0);
			if (status == 2)
			{
				if (i != shell->num_pipe)
					shell->signal = 1;
			}
		}
	}
	return (status);
}
