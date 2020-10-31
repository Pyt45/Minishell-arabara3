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
		{
			if (ft_isalpha((int)(cmds->args[1][i])))
			{
				tstatus = 1;
				break ;
			}
		}
		status = ft_atoi(cmds->args[1]);
	}
	free_shell(shell);
	ft_free_arr(shell->env);
	ft_putstr_fd("exit\n", 1);
	(tstatus && !status) ? print_error("exit", 33, 0) : 0;
	exit(status);
	return (0);
}


int			command_line(t_shell *shell)
{
	int		r;
	int		status;

	while (status)
	{
		if (shell->ret != 130)
			ft_putstr_fd("\033[0;33mminishell~>\033[0m", 1);
		r = get_next_line(0, &shell->line);
		if (ft_strlen(shell->line))
			status = run_commands(shell);
	}
	return (status);
}

void	sig_handle_ctrl_c(int sig)
{
	if (sig == SIGINT)
	{	
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("\033[0;33mminishell~>\033[0m", 1);
	}
}

int     main(int argc, char **argv, char **envp)
{
	t_shell shell;

	erase_file_debug();
	signal(SIGINT, sig_handle_ctrl_c);
	if (argc && argv)
	{
		init_shell(&shell);
		shell.env = ft_arrdup(envp);
		while (command_line(&shell))
			;
	}
    return (0);
}
