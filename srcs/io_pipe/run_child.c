/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 09:34:20 by aaqlzim           #+#    #+#             */
/*   Updated: 2020/11/28 18:52:24 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void			check_file(char *file, int cas, t_shell *shell)
{
	struct stat	file_stat;

	if (cas == 1)
	{
		if (stat(file, &file_stat) < 0)
			exit_error(strerror(errno), 1, shell);
		if (!ft_access(file, 1))
			exit_error("Permission denied", 1, shell);
	}
	else if (cas == 2)
	{
		stat(file, &file_stat);
		if (file_stat.st_mode == 16877)
			exit_error("its a directory", 1, shell);
		if (stat(file, &file_stat) < 0)
			exit_error(strerror(errno), 1, shell);
	}
}

static void		exec_help(t_shell *shell, t_cmds *cmds)
{
	char		*cmd;

	if ((cmds->cmd[0] == '.' && cmds->cmd[1] == '/') ||
		ft_strchr(cmds->cmd, '/'))
	{
		if (cmds->cmd[ft_strlen(cmds->cmd) - 1] == '/')
			check_file(cmds->cmd, 2, shell);
		else
			check_file(cmds->cmd, 1, shell);
		execve(cmds->cmd, cmds->args, shell->env);
	}
	else
	{
		cmd = get_bin_path(cmds->cmd, shell->env);
		execve(cmd, cmds->args, shell->env);
	}
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
