/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 12:27:22 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/07 12:10:52 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid)
{
	int		status;

	status = 0;
	status = wait_child(shell, pid, status);
	cmds->ret = get_status_number(status);
}

static t_cmds	*excute_loop_append(t_cmds *cmds)
{
	while (cmds->append)
		cmds = cmds->next;
	return (cmds);
}

t_cmds			*excute_command_by_order(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;

	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		shell->exec.tmpin = dup(0);
		shell->exec.tmpout = dup(1);
		if (cmds->append >= 0)
			shell->exec.fdin = dup(shell->exec.tmpin);
		while (cmds)
		{
			if (cmds->end && cmds->prev && cmds->prev->append)
				break ;
			pid = run_child(shell, cmds);
			cmds = excute_loop_append(cmds);
			if (cmds->end || !cmds->next)
				break ;
			else
				cmds = cmds->next;
		}
		dup2(shell->exec.tmpin, 0);
		dup2(shell->exec.tmpout, 1);
		close(shell->exec.tmpin);
		close(shell->exec.tmpout);
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
