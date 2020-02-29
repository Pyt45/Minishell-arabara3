

#include "../includes/shell.h"


int		command_line(t_shell *shell)
{
	int		r;
	int		status;

	r = 1;
	status = 1;
	
	while (status)
	{
		ft_putstr_fd("minishell~>", 1);
		r = get_next_line(0, &shell->line);
		status = excute_command_by_order(shell);
		free(shell->line);
	}
	return (status);
}

void	sig_handle_ctrl_c(int signal)
{
	if (signal)
		exit(EXIT_FAILURE);
}

int     main(int argc, char **argv, char **envp)
{
	t_shell shell;

	if (argc && argv)
	{
		shell.env = ft_arrdup(envp);
		//signal(SIGINT, sig_handle_ctrl_c);
		while (command_line(&shell))
			;
	}
    return (0);
}