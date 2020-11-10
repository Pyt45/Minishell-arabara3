/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 11:15:58 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/10 09:46:37 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		get_num_pipes(t_cmds *cmds)
{
	int		i;
	t_cmds	*tmp;

	i = 0;
	tmp = cmds;
	while (tmp->p)
	{
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

int		get_status_number(int status)
{
	if (status > 200)
		return (status - 129);
	else if (status == 2)
		return (130);
	else if (status == 3)
		return (131);
	return (status);
}

int		wait_child(t_shell *shell, pid_t pid, int st)
{
	int		i;

	i = -1;
	if (!shell->num_pipe)
		waitpid(pid, &st, 0);
	else
	{
		while (++i < 2 * shell->num_pipe)
			wait(&st);
	}
	return (st);
}
