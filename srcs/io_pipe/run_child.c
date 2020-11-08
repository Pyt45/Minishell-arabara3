/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 09:34:20 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/07 12:11:44 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	exec_help(t_shell *shell, t_cmds *cmds)
{
	if (cmds->cmd[0] == '/' || (cmds->cmd[0] == '.' && cmds->cmd[1] == '/'))
		execve(cmds->cmd, cmds->args, shell->env);
	else
		execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env);
}

int			exec_commands(t_shell *shell, t_cmds *cmds)
{
	int		ret;

	ret = 1;
	if (!cmds->cmd || !cmds->cmd[0])
		return (0);
	cmds->cmd = clear_quotes(cmds->cmd);
	if (!ft_strcmp(cmds->cmd, "env"))
		ret = env_builtin(cmds, shell->env);
	else if (!ft_strcmp(cmds->cmd, "cd"))
		ret = cd_builtin(shell, cmds);
	else if (!ft_strcmp(cmds->cmd, "pwd"))
		ret = pwd_builtin();
	else if (!ft_strcmp(cmds->cmd, "exit"))
		ret = exit_builtin(shell, cmds);
	else if (!ft_strcmp(cmds->cmd, "export"))
		ret = export_builtin(shell, cmds);
	else if (!ft_strcmp(cmds->cmd, "unset"))
		ret = unset_builtin(shell, cmds);
	else if (!ft_strcmp(cmds->cmd, "echo"))
		ret = echo_builtin(cmds);
	else if (!cmds->prev || !cmds->prev->append)
		exec_help(shell, cmds);
	else
		ret = 0;
	return (ret);
}

static void	child_help(t_shell *shell, t_cmds *cmds)
{
	if (cmds->append >= 0)
	{
		// shell->exec.fdin = dup(shell->exec.tmpin);
		dup2(shell->exec.fdin, 0);
		close(shell->exec.fdin);
	}
	// if (cmds->append || (cmds->prev && cmds->prev->append))
	if (cmds->append)
	{
		exec_io_redi(shell, cmds);
		// for cat < out | echo a
		if (cmds->append < 0)
			cmds->next = cmds->next->next;
		if (cmds->append < 0 && cmds->next)
		{
			int	fdpipe[2];
			pipe(fdpipe);
			shell->exec.fdout = fdpipe[1];
			shell->exec.fdin = fdpipe[0];
		}
	}
	else if (cmds->end)
		shell->exec.fdout = dup(shell->exec.tmpout);
	else if (cmds->next)
	{
		write_to_file("PIPE ", cmds->cmd, 1);
		int	fdpipe[2];
		pipe(fdpipe);
		shell->exec.fdout = fdpipe[1];
		shell->exec.fdin = fdpipe[0];
	}
	// write_to_file("IN ", ft_itoa(shell->exec.fdin), 1);
	// write_to_file("OUT ", ft_itoa(shell->exec.fdout), 1);
	dup2(shell->exec.fdout, 1);
	close(shell->exec.fdout);
}

pid_t		run_child(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;
	
	child_help(shell, cmds);
	pid = fork();
	if (pid == 0)
	{
		if (cmds->args && exec_commands(shell, cmds) && !is_builtin(cmds->cmd))
		{
			print_error(cmds->cmd, errno, 1);
			exit(1);
		}
		exit(0);
	}
	else if (pid < 0)
	{
		print_error("fork", errno, 0);
		exit(1);
	}
	return (pid);
}
