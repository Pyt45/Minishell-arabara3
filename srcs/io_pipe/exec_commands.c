/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:10:18 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/12 10:56:05 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid, int *pids)
{
	int		status;

	status = 0;
	close_pipes(shell->exec.fds, shell->num_pipe);
	status = wait_child(shell, pid, pids);
	
	// if (WIFSIGNALED(status))
	if (status == 2 || status == 3)
		cmds->ret = (status & 0177) + 128;
	else
		cmds->ret = (status >> 8) & 0x000000ff;
	write_to_file("Status ", ft_itoa(cmds->ret), 1);
	if (shell->num_pipe)
		free(shell->exec.fds);
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

void			restore_fds(int *fds)
{
	dup2(fds[0], 0);
	close(fds[0]);
	dup2(fds[1], 1);
	close(fds[1]);
	dup2(fds[2], 2);
	close(fds[2]);
}

t_cmds			*excute_command_by_order(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;
	int		*pids;
	int i;

	i = 0;
	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		pids = malloc(sizeof(int) * (shell->num_pipe + 1));
		shell->exec.fds = pipe_fds(shell->num_pipe, shell->exec.fds);
		save_fds(shell->exec.backup);
		while (cmds)
		{
			if (!cmds->skip)
			{
				pid = run_child(shell, cmds);
				write_to_file("PID ", ft_itoa(pid), 1);
				pids[i++] = pid;
				cmds = excute_loop_append(cmds);
				shell->exec.j += 2;
			}
			if (cmds->end)
				break ;
			else
				cmds = cmds->next;
		}
		restore_fds(shell->exec.backup);
		excute_cmd_help(shell, cmds, pid, pids);
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
			cmds = excute_command_by_order(shell, cmds);
			shell->ret = cmds->ret;
			cmds = cmds->next;
		}
	}
	return (1);
}
