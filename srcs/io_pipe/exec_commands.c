/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:10:18 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/21 12:36:19 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid)
{
	int		status;
	float	shift;

	status = 0;
	close_pipes(shell->exec.fds, shell->num_pipe);
	status = wait_child(shell, pid);
	if (status == 2 || status == 3)
		cmds->ret = status + 128;
	else
	{
		shift = (status) / (ft_pow(2, 8));
		cmds->ret = (int)shift & 255;
	}
	if (shell->num_pipe)
	{
		ft_del(shell->exec.fds);
		ft_del(shell->exec.pids);
	}
}

static t_cmds	*excute_loop_append(t_cmds *cmds)
{
	while (cmds && cmds->append)
	{
		if (!cmds->next)
			break ;
		cmds = cmds->next;
	}
	return (cmds);
}

void			save_fds(int *fds)
{
	fds[0] = dup(0);
	fds[1] = dup(1);
	fds[2] = dup(2);
}

t_cmds			*excute_command_by_order(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;

	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		shell->exec.fds = pipe_fds(shell->num_pipe, shell->exec.fds);
		save_fds(shell->exec.backup);
		while (cmds)
		{
			if (!cmds->skip)
			{
				pid = run_child(shell, cmds);
				cmds = excute_loop_append(cmds);
				shell->exec.j += 2;
			}
			if (cmds->end)
				break ;
			else
				cmds = cmds->next;
		}
		restore_fds(shell->exec.backup);
		excute_cmd_help(shell, cmds, pid);
	}
	else if (cmds->cmd)
	{
		cmds->ret = exec_commands(shell, cmds);
	}
	return (cmds);
}

void			run_commands(t_shell *shell)
{
	t_cmds	*cmds;

	shell->num_pipe = 0;
	shell->parse_err = 0;
	shell = parse_commands(shell);
	g_ret = !g_ret ? 2 : g_ret;
	if (check_parsing(shell))
	{
		cmds = shell->cmds;
		while (cmds)
		{
			if (cmds->line)
				parse_command(shell, cmds);
			if (!check_parsing(shell))
				break ;
			shell->exec.j = 0;
			shell->num_pipe = get_num_pipes(cmds);
			if (shell->num_pipe)
				shell->exec.pids = malloc(sizeof(int) * (shell->num_pipe + 1));
			cmds = excute_command_by_order(shell, cmds);
			shell->ret = cmds->ret;
			cmds = cmds->next;
		}
	}
}
