/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:04:49 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/14 16:35:06 by zlayine          ###   ########.fr       */
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
	ft_putstr_fd("exit\n", 1);
	(tstatus && !status) ? print_error("exit", 33, 0) : 0;
	ft_del(shell);
	exit(status);
	return (0);
}

void	sig_handle_ctrl_c(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("\033[0;33mminishell~>\033[0m", 1);
	}
	else if (sig == SIGQUIT)
	{
		ft_putendl_fd("Quit: 3", 1);
		return ;
	}
}

int		command_line(t_shell *shell)
{
	int		r;
	int		status;

	status = 1;
	while (status)
	{
		signal(SIGQUIT, SIG_IGN);
		if (shell->ret != 130)
			ft_putstr_fd("\033[0;33mminishell~>\033[0m", 1);
		r = get_next_line(0, &shell->line);
		if (r == 0)
			exit_builtin(shell, shell->cmds);
		if (ft_strlen(shell->line))
			status = run_commands(shell);
		ft_del(shell->line);
		free_shell(shell);
	}
	return (status);
}

int		main(int argc, char **argv, char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	signal(SIGINT, sig_handle_ctrl_c);
	if (argc && argv)
	{
		init_shell(shell);
		shell->env = ft_arrdup(envp);
		command_line(shell);
	}
	ft_free_arr(shell->env);
	return (0);
}
