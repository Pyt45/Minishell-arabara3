#include "../includes/shell.h"

int			command_line(t_shell *shell)
{
	int		r;
	int		status;

	while (status)
	{
		if (shell->ret != 2)
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
