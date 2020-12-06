/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:04:49 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/20 10:20:06 by aaqlzim          ###   ########.fr       */
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
		ft_putstr_fd("\b\b  \b\b\n\033[1;36mminishell\033[0m\033[1;31m~>\033[0m", 1);
	}
	else if (sig == SIGQUIT)
	{
		if (g_ret == 2)
			ft_putendl_fd("Quit: 3", 2);
		else
			ft_putstr_fd("\b\b  \b\b", 1);
	}
}

static void	make_line(t_config *config, t_shell *shell, int save)
{
	char	*tmp;

	if (save)
	{
		ft_putstr_fd("  \b\b", 1);
		tmp = config->tmp ? ft_strjoin(config->tmp, shell->line) :
			ft_strdup(shell->line);
		ft_del(config->tmp);
		config->tmp = tmp;
		ft_del(shell->line);
	}
	else
	{
		if (config->tmp && g_ret != 1 && shell->signal != 1)
		{
			tmp = ft_strjoin(config->tmp, shell->line);
			ft_del(shell->line);
			shell->line = tmp;
		}
		if (config->tmp)
		{
			ft_del(config->tmp);
			config->tmp = NULL;
		}
	}
}

void		command_line(t_shell *shell)
{
	int		r;

	while (1)
	{
		if (shell->ret != 130 && shell->signal != 1 && r != 0)
		{
			if (shell->ret == 0)
				ft_putstr_fd("\033[1;36mminishell\033[0m\033[1;32m~>\033[0m", 1);
			else
				ft_putstr_fd("\033[1;36mminishell\033[0m\033[1;31m~>\033[0m", 1);
		}
		r = get_next_line(0, &shell->line);
		make_line(&shell->config, shell, 0);
		// printf("shel line %d\n", r);
		shell->signal = 0;
		if (r == 0 && !ft_strlen(shell->line) && !shell->config.tmp)
			exit_builtin(shell, shell->cmds);
		else if (r == 0)
			make_line(&shell->config, shell, 1);
		else if (r != 0)
		{
			if (ft_strlen(shell->line))
				run_commands(shell);
			ft_del(shell->line);
			free_shell(shell);
			g_ret = 0;
		}
	}
}

int			main(int argc, char **argv, char **envp)
{
	t_shell *shell;
	int		i;

	shell = malloc(sizeof(t_shell));
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, sig_handle);
	g_ret = 0;
	shell->ret = 0;
	shell->signal = 0;
	shell->config.tmp = NULL;
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
