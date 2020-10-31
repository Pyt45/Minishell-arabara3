/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 12:27:22 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/10/31 14:25:38 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid)
{
	int		status;

	status = 0;
	close_pipes(shell->fds, shell->num_pipe);
	status = wait_child(shell, pid, status);
	cmds->ret = get_status_number(status);
	free(shell->fds);
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
	int		*ior;
	int		j;

	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		shell->fds = pipe_fds(shell->num_pipe, shell->fds);
		j = 0;
		while (cmds)
		{
			if (cmds->end && cmds->prev && cmds->prev->append)
				break ;
			pid = run_child(shell, cmds, j);
			cmds = excute_loop_append(cmds);
			if (cmds->end)
				break ;
			else
				cmds = cmds->next;
			j += 2;
		}
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
