#include "../includes/shell.h"



int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
    double  tstatus;
    int     status;

	tstatus = 0;
	if (cmds->args[1] != NULL)
        status = ft_atoi(cmds->args[1]);
    end_terminal(&shell->config);
    exit(status);
    //ft_free_arr(shell->env);
    return (1);
}