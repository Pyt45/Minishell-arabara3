/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 09:34:20 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/02 10:27:28 by aaqlzim          ###   ########.fr       */
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

static void	child_help(t_shell *shell, t_cmds *cmds, int *ior)
{
	ior[0] = 0;
	ior[1] = shell->fds[0];
	exec_io_redi(cmds, ior[0], ior[1]);
	if (cmds->args && exec_commands(shell, cmds))
	{
		print_error(cmds->cmd, errno, 1);
		exit(1);
	}
	if (ior[1] != shell->fds[1])
		close(shell->fds[1]);
	ior[1] = 1;
}

pid_t		run_child(t_shell *shell, t_cmds *cmds, int j)
{
	pid_t	pid;
	int		ior[2];

	ior[1] = 0;
	pid = fork();
	if (pid == 0)
	{
		(shell->num_pipe) ? shell->fds = create_fds(cmds, j, shell->fds) : 0;
		close_pipes(shell->fds, shell->num_pipe);
		if (cmds->append || (cmds->prev && cmds->prev->append))
			child_help(shell, cmds, ior);
		else if (cmds->args && exec_commands(shell, cmds) &&
		!is_builtin(cmds->cmd))
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
