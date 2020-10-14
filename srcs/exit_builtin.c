#include "../includes/shell.h"



int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
    double  tstatus;
    int     status;

	tstatus = 0;
	if (cmds->args[1] != NULL)
        tstatus = ft_fatoi(cmds->args[1]);
    status = ft_fatoi(cmds->args[1]);
    if (status != tstatus)
    {
        ft_putstr_fd("Invalid argument to exit\n", 2);
        exit(128);
    }
    else
        exit(status);
    //ft_free_arr(shell->env);
    return (1);
}