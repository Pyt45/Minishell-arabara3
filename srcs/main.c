/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:04:49 by zlayine           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2020/11/18 09:35:38 by aaqlzim          ###   ########.fr       */
=======
/*   Updated: 2020/11/17 18:17:56 by zlayine          ###   ########.fr       */
>>>>>>> 6894403f6a1c0ac14a537eb625e43480b1a5fb73
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
	double	tstatus;
	int		status;
	int		i;

	tstatus = 0;
	status = 0;
	i = -1;
	if (cmds && cmds->args[1])
	{
		while (cmds->args[1][++i])
			if (ft_isalpha((int)(cmds->args[1][i])))
				tstatus = 1;
		status = ft_atoi(cmds->args[1]);
	}
	ft_del(shell->line);
	free_shell(shell);
	ft_free_arr(shell->env);
	ft_putstr_fd("exit\n", 2);
	(tstatus && !status) ? print_error("exit", 33, 0) : 0;
	ft_del(shell);
	exit(status);
	return (0);
}

void	sig_handle(int sig)
{
	if (sig == SIGINT)
	{
<<<<<<< HEAD
		ft_putstr_fd("\n", 1);
		//if (g_ret == 0)
			ft_putstr_fd("\033[0;33mminishell~>\033[0m", 1);
		g_ret = 1;
=======
		g_ret = 1;
		ft_putstr_fd("\n", 2);
		ft_putstr_fd("\033[0;33mminishell~>\033[0m", 2);
>>>>>>> 6894403f6a1c0ac14a537eb625e43480b1a5fb73
	}
	else if (sig == SIGQUIT)
	{
		ft_putendl_fd("Quit: 3", 2);
		return ;
	}
}

void	command_line(t_shell *shell)
{
	int		r;
	int		status;

	status = 1;
	while (status)
	{
		signal(SIGQUIT, SIG_IGN);
<<<<<<< HEAD
		signal(SIGINT, sig_handle);
		if (shell->ret != 130 && shell->signal != 1)
			ft_putstr_fd("\033[0;33mminishell~>\033[0m", 1);
		shell->signal = 0;
=======
		if (shell->ret != 130)
			ft_putstr_fd("\033[0;33mminishell~>\033[0m", 2);
>>>>>>> 6894403f6a1c0ac14a537eb625e43480b1a5fb73
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


int		main(int argc, char **argv, char **envp)
{
	t_shell *shell;
	int		i;

	shell = malloc(sizeof(t_shell));
	signal(SIGINT, sig_handle);
	erase_file_debug();
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
