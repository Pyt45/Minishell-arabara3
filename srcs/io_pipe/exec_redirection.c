/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 11:03:45 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/10/31 11:14:56 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	redirect_forward(t_cmds *tmp, t_cmds *cmd, int *fd)
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
	if (fd[1])
		close(fd[1]);
	fd[1] = open_output(tmp, tmp->append - 1);
}

void	redirect_backward(t_cmds *tmp, int *fd)
{
	int		i;
	char	*file;

	i = -1;
	if (fd[0])
		close(fd[0]);
	while (tmp->next->args[++i])
		file = tmp->next->args[i];
	fd[0] = open_input(file, 0, fd[0]);
}

void	do_redirect(t_cmds *cmd, int *fd)
{
	t_cmds *tmp;

	tmp = cmd;
	while (tmp->append)
	{
		if (tmp->append > 0)
			redirect_forward(tmp, cmd, fd);
		else
			redirect_backward(tmp, fd);
		tmp = tmp->next;
	}
	if (fd[1])
		dup2(fd[1], 1);
	if (fd[0])
		dup2(fd[0], 0);
}

void	exec_io_redi(t_cmds *cmd, int ifd, int ofd)
{
	int		new_fd[2];

	new_fd[0] = ifd;
	new_fd[1] = ofd;
	do_redirect(cmd, new_fd);
	if (new_fd[0] != ifd)
		close(new_fd[0]);
	if (new_fd[1] != ofd)
		close(new_fd[1]);
}
