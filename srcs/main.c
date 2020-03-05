#include "../includes/shell.h"

// void	free_shell(t_shell *shell)
// {
// 	t_s_cmds *s_cmd;
// 	t_m_cmds *m_cmd;
// 	t_b_cmds *b_cmd;
// 	int i;
// 	// free all the data and re-init
// 	// free(shell->line);
// 	while(shell->cmds)
// 	{
// 		// puts("FREE B_CMDS");
// 		while (shell->cmds->m_cmds)
// 		{
// 			// puts("\tFREE M_CMDS");
// 			while (shell->cmds->m_cmds->s_cmds)
// 			{
// 				// puts("\t\tFREE S_CMDS");
// 				i = 0;
// 				while (shell->cmds->m_cmds->s_cmds->args[i])
// 				{
// 					// printf("\t\t\tARG DELETED: %s\n", shell->cmds->m_cmds->s_cmds->args[i]);
// 					ft_del(*(shell->cmds->m_cmds->s_cmds->args));
// 					i++;
// 				}
// 				ft_del(shell->cmds->m_cmds->s_cmds->args);
// 				ft_del(shell->cmds->m_cmds->s_cmds->cmd);
// 				s_cmd = shell->cmds->m_cmds->s_cmds->next;
// 				ft_del(shell->cmds->m_cmds->s_cmds);
// 				shell->cmds->m_cmds->s_cmds = s_cmd;
// 			}
// 			ft_del(shell->cmds->m_cmds->cmd);
// 			m_cmd = shell->cmds->m_cmds->next;
// 			ft_del(shell->cmds->m_cmds);
// 			shell->cmds->m_cmds = m_cmd;
// 		}
// 		ft_del(shell->cmds->cmd);
// 		b_cmd = shell->cmds->next;
// 		ft_del(shell->cmds);
// 		shell->cmds = b_cmd;
// 	}
// 	shell->cmds = NULL;
// }
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
		status = run_commands(shell);
		//free_shell(shell);
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