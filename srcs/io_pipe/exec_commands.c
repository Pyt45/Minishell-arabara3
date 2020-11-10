/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 12:27:22 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/10 12:11:20 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid)
{
	int		status;

	status = 0;
	status = wait_child(shell, pid, status);
	if (WIFEXITED(status))
		cmds->ret = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		cmds->ret = WTERMSIG(status) + 128;
	//cmds->ret = get_status_number(status);
}

static t_cmds	*excute_loop_append(t_cmds *cmds)
{
	while (cmds && cmds->append > 0)
	{
		if (!cmds->next)
			break;
		cmds = cmds->next;
	}
	return (cmds);
}

void	save_fds(int *fds)
{
	fds[0] = dup(0);	
	fds[1] = dup(1);	
	fds[2] = dup(2);	
}

void	restore_fds(int *fds)
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

	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		shell->exec.fds = pipe_fds(shell->num_pipe, shell->exec.fds);
		save_fds(shell->exec.backup);
		shell->exec.j = 0;
		while (cmds)
		{
			if (!cmds->skip)
			{
				write_to_file("CMD ", cmds->cmd, 1);
				pid = run_child(shell, cmds);
				cmds = excute_loop_append(cmds);
				if (cmds->p)
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
			shell->num_pipe = get_num_pipes(cmds);
			cmds = excute_command_by_order(shell, cmds);
			shell->ret = cmds->ret;
			cmds = cmds->next;
		}
	}
	return (1);
}