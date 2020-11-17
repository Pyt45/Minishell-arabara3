/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 09:34:20 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/17 17:37:58 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static	void	exec_help(t_shell *shell, t_cmds *cmds)
{
	if (cmds->cmd[0] == '/' || (cmds->cmd[0] == '.' && cmds->cmd[1] == '/'))
		execve(cmds->cmd, cmds->args, shell->env);
	else
		execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env);
}

int				exec_commands(t_shell *shell, t_cmds *cmds)
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
	else
		exec_help(shell, cmds);
	cmds->ret = ret;
	return (ret);
}

pid_t			run_child(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		shell->exec.fdin = 0;
		shell->exec.fdout = 0;
		(shell->num_pipe) ? shell->exec.fds = create_fds(cmds, shell->exec.j,
			shell->exec.fds) : 0;
		close_pipes(shell->exec.fds, shell->num_pipe);
		if (cmds->append)
			exec_io_redi(shell, cmds);
		if (cmds->args && !cmds->ret &&
			exec_commands(shell, cmds) && !is_builtin(cmds->cmd))
			exit_error(cmds->cmd, 127, shell);
		exit(cmds->ret);
	}
	else if (pid < 0)
		exit_error("Fork error", 1, shell);
	if (shell->num_pipe)
		shell->exec.pids[shell->exec.j / 2] = pid;
	return (pid);
}
