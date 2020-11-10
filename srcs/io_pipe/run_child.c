/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 09:34:20 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/10 11:53:55 by zlayine          ###   ########.fr       */
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
	if (cmds->append)
	{
		exec_io_redi(shell, cmds);
		// if (cmds->append < 0)
		// {
		// 	if (cmds->next)
		// 	{
		// 		cmds->next->skip = 1;
		// 		cmds->end = 1;
		// 	}
		// 	else if (cmds->next->p)
		// 	{
		// 		cmds->p = 1;
		// 		cmds->next->skip = 1;
		// 	}
		// }
	}
}

// static void	child_help(t_shell *shell, t_cmds *cmds)
// {
// 	int	fdpipe[2];

// 	write_to_file("CMD ", cmds->cmd, 1);
// 	if (cmds->append >= 0)
// 	{
// 		dup2(shell->exec.fdin, 0);
// 		close(shell->exec.fdin);
// 	}
	
// 	if (cmds->append)
// 	{
// 		exec_io_redi(shell, cmds);
// 		// write_to_file("FDi io ", ft_itoa(shell->exec.fdin), 1);
// 		// write_to_file("FDo io ", ft_itoa(shell->exec.fdout), 1);
// 		if (cmds->append < 0)
// 		{
// 			if (cmds->next->end)
// 			{
// 				cmds->next->skip = 1;
// 				// cmds->end = 1;
// 			}
// 			else if (cmds->next->p)
// 			{
// 				cmds->p = 1;
// 				cmds->next->skip = 1;
// 			}
// 			// cmds->next = cmds->next->next;
// 			// this does not work if the next doesnt exist (the free must be done before)
// 			// if (cmds->next)
// 			// {
// 			// 	ft_del(cmds->next->prev);
// 			// 	cmds->next->prev = cmds;
// 			// }
// 		}
// 	}
// 	if (cmds->end)
// 		shell->exec.fdout = dup(shell->exec.tmpout);
// 	else if (cmds->p)
// 	{
// 		pipe(fdpipe);
// 		shell->exec.fdin = fdpipe[0];
// 		shell->exec.fdout = fdpipe[1];
// 	}
// 	write_to_file("FDi a ", ft_itoa(shell->exec.fdin), 1);
// 	write_to_file("FDo a ", ft_itoa(shell->exec.fdout), 1);
// 	dup2(shell->exec.fdout, 1);
// 	close(shell->exec.fdout);
// }

pid_t		run_child(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;
	
	// child_help(shell, cmds);
	pid = fork();
	if (pid == 0)
	{
		(shell->num_pipe) ? shell->exec.fds = create_fds(cmds, shell->exec.j, shell->exec.fds) : 0;
		close_pipes(shell->exec.fds, shell->num_pipe);
		shell->exec.fdin = 0;
		shell->exec.fdout = 0;
		child_help(shell, cmds);
		if (cmds->args && exec_commands(shell, cmds) && !is_builtin(cmds->cmd))
		{
			// print_error(cmds->cmd, errno, 1);
			exit_error(cmds->cmd, 127, shell);
			// exit(1);
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
