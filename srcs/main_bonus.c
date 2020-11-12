/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 09:59:19 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/12 14:25:06 by aaqlzim          ###   ########.fr       */
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
	if (cmds->args[1])
	{
		while (cmds->args[1][++i])
			if (ft_isalpha((int)(cmds->args[1][i])))
				tstatus = 1;
		status = ft_atoi(cmds->args[1]);
	}
	if (!shell->num_pipe)
		end_terminal(&shell->config);
	free_shell(shell);
	free_config(&shell->config);
	ft_free_arr(shell->env);
	ft_putstr_fd("exit\n", 1);
	if (tstatus && !status)
		print_error("exit", 33, 0);
	exit(status);
	return (0);
}

char	*clear_str(char *str)
{
	int i;
	int j;

	i = -1;
	j = 0;
	while (str[++i])
		if (str[i] == ' ')
		{
			j = i;
			while (str[j])
			{
				str[j] = str[j + 1];
				j++;
			}
			i--;
		}
		else
			break ;
	return (str);
}

char	*read_line(t_shell *shell)
{
	init_prompt(&shell->config, shell->ret);
	while ((*shell->config.str && shell->config.tmp) ||
		shell->config.buff || read(0, &shell->config.buff,
			sizeof(&shell->config.buff)))
	{
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
	while ((shell->line = read_line(shell)))
	{
		if (ft_strlen(shell->line))
			run_commands(shell);
		free_shell(shell);
	}
}

void	sig_handle_ctrl_c(int sig)
{
	sig = 0;
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("\033[1;32mminishell~>\033[0m", 1);
}

int		main(int argc, char **argv, char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	erase_file_debug();
	signal(SIGINT, sig_handle_ctrl_c);
	if (argc && argv)
	{
		init_shell(shell);
		shell->env = ft_arrdup(envp);
		command_line(shell);
	}
	return (0);
}
