/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:10:18 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/14 14:06:00 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid)
{
	int		status;

	status = 0;
	close_pipes(shell->exec.fds, shell->num_pipe);
	status = wait_child(shell, pid);
	if (status == 2 || status == 3)
		cmds->ret = (status & 0177) + 128;
	else
		cmds->ret = (status >> 8) & 0x000000ff;
	if (shell->num_pipe)
	{
		ft_del(shell->exec.fds);
		ft_del(shell->exec.pids);
	}
}

static t_cmds	*excute_loop_append(t_cmds *cmds)
{
	while (cmds && cmds->append > 0)
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
		cmds->ret = exec_commands(shell, cmds);
	return (cmds);
}

int				run_commands(t_shell *shell)
{
	t_cmds	*cmds;

	shell->num_pipe = 0;
	shell->parse_err = 0;
	shell = parse_commands(shell);
	if (shell->parse_err == -1)
		print_error("syntax error", 0, 0);
	else
	{
		cmds = shell->cmds;
		while (cmds)
		{
			signal(SIGQUIT, sig_handle_ctrl_c);
			shell->exec.j = 0;
			shell->num_pipe = get_num_pipes(cmds);
			if (shell->num_pipe)
				shell->exec.pids = malloc(sizeof(int) * (shell->num_pipe + 1));
			cmds = excute_command_by_order(shell, cmds);
			shell->ret = cmds->ret;
			cmds = cmds->next;
		}
	}
	return (1);
}
