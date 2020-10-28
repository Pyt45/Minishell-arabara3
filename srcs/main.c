#include "../includes/shell.h"
#include <signal.h>
// #include <readline/readline.h>

// void	free_shell(t_shell *shell)
// {
// 	int i;
// 	t_cmds *tmp;
// 	// free all the data and re-init
// 	//free(shell->line);
// 	while(shell->cmds)
// 	{
// 		i = 0;
// 		while (shell->cmds->args[i])
// 		{
// 			ft_del(*(shell->cmds->args));
// 			i++;
// 		}
// 		ft_del(shell->cmds->args);
// 		ft_del(shell->cmds->cmd);
// 		tmp = shell->cmds->next;
// 		ft_del(shell->cmds);
// 		shell->cmds = tmp;
// 	}
// 	shell->cmds = NULL;
// }

// void		init_shell(t_shell *shell)
// {
// 	shell->line = NULL;
// 	shell->parse_err = 0;
// 	shell->ret = 0;
// }

// int		exit_builtin(t_shell *shell, t_cmds *cmds)
// {
//     double  tstatus;
//     int     status;

// 	tstatus = 0;
// 	if (cmds->args[1] != NULL)
//         status = ft_atoi(cmds->args[1]);
//     // ft_free_arr(shell->env);
//     exit(status);
//     return (0);
// }

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
