/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 09:59:19 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/20 10:19:50 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
	long	status;
	int		i;
	int		is_cmd;

	if (!check_len(cmds))
		return (1);
	status = 0;
	is_cmd = cmds && cmds->start ? 1 : 0;
	i = -1;
	if (cmds && cmds->args[1])
		status = valid_status(cmds->args[1]);
	(!shell->num_pipe) ? end_terminal(&shell->config) : 0;
	(is_cmd) ? free_config(&shell->config) : 0;
	(is_cmd) ? ft_free_arr(shell->env) : 0;
	(is_cmd || !cmds) ? ft_putstr_fd("exit\n", 2) : 0;
	(status > 200 && status < 300) ? print_error("exit", 33, 0) : 0;
	(is_cmd) ? free_shell(shell) : 0;
	(is_cmd || !cmds) ? ft_del(shell) : 0;
	exit(status);
	return (0);
}

char	*read_line(t_shell *shell)
{
	init_prompt(&shell->config, shell->ret);
	while ((*shell->config.str && shell->config.tmp) ||
		shell->config.buff || read(0, &shell->config.buff,
			sizeof(&shell->config.buff)))
	{
		if (shell->config.buff == 4 && !ft_strlen(shell->config.str))
			exit_builtin(shell, shell->cmds);
		validate_cursor(&shell->config, shell);
		handle_keys(&shell->config);
		if (ft_isprint(shell->config.buff))
			print_char(&shell->config);
		if (shell->config.buff == ENTER_BTN)
		{
			shell->config.str = clear_str(shell->config.str);
			shell->line = shell->config.str;
			newline_config(&shell->config, 0);
			end_terminal(&shell->config);
			shell->config.buff = 0;
			break ;
		}
		shell->config.buff = 0;
	}
	return (shell->line);
}

void	command_line(t_shell *shell)
{
	shell->ret = 0;
	shell->config.history = NULL;
	while ((shell->line = read_line(shell)))
	{
		if (ft_strlen(shell->line))
			run_commands(shell);
		free_shell(shell);
		init_control(&shell->config.control, shell->config.control.cpy ||
			shell->config.control.cut);
	}
}

void	sig_handle(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 2);
		ft_putstr_fd("\033[1;34mminishell\033[0m\033[1;31m~>\033[0m", 1);
	}
	else if (sig == SIGQUIT)
	{
		ft_putendl_fd("Quit: 3", 2);
		return ;
	}
}

int		main(int argc, char **argv, char **envp)
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
	erase_file_debug();
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
