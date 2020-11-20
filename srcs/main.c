/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:04:49 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/20 09:47:19 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int			exit_builtin(t_shell *shell, t_cmds *cmds)
{
	double	tstatus;
	long	status;
	int		i;

	if (!check_len(cmds))
		return (1);
	tstatus = 1;
	status = 0;
	i = -1;
	if (cmds && cmds->args[1])
	{
		status = ft_atoi_l(cmds->args[1]);
		tstatus = valid_status(cmds->args[1], status);
	}
	ft_del(shell->line);
	free_shell(shell);
	ft_free_arr(shell->env);
	ft_putstr_fd("exit\n", 2);
	(!tstatus) ? print_error("exit", 33, 0) : 0;
	ft_del(shell);
	exit(status);
	return (0);
}

void		sig_handle(int sig)
{
	if (sig == SIGINT)
	{
		g_ret = 1;
		ft_putstr_fd("\n", 2);
		ft_putstr_fd("\033[0;33mminishell~>\033[0m", 2);
	}
	else if (sig == SIGQUIT)
	{
		ft_putendl_fd("Quit: 3", 2);
		return ;
	}
}

void		command_line(t_shell *shell)
{
	int		r;
	int		status;

	status = 1;
	while (status)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_handle);
		if (shell->ret != 130 && shell->signal != 1)
			ft_putstr_fd("\033[0;33mminishell~>\033[0m", 1);
		shell->signal = 0;
		r = get_next_line(0, &shell->line);
		if (r == 0)
			exit_builtin(shell, shell->cmds);
		if (ft_strlen(shell->line))
			run_commands(shell);
		ft_del(shell->line);
		free_shell(shell);
		g_ret = 0;
	}
}

int			main(int argc, char **argv, char **envp)
{
	t_shell *shell;
	int		i;

	shell = malloc(sizeof(t_shell));
	signal(SIGINT, sig_handle);
	g_ret = 0;
	if (argc && argv)
	{
		init_shell(shell);
		shell->env = ft_arrdup(envp);
		if ((i = ft_getenv("OLDPWD", shell->env)) >= 0)
			shell->env = ft_remove_from_arr(i, shell->env);
		command_line(shell);
	}
	return (0);
}
