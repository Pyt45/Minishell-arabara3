#include "../../includes/shell.h"

int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
    double  tstatus;
    int     status;
    int     i;

	tstatus = 0;
    i = 0;
	if (cmds->args[1] != NULL)
    {
        while (cmds->args[1][i])
        {
            if (ft_isalpha((int)(cmds->args[1][i])))
            {
                tstatus = 1;
                break;
            }
            i++;
        }
        status = ft_atoi(cmds->args[1]);
        if (tstatus && status == 0)
        {
            ft_putstr_fd("exit\n", 1);
            print_error("exit", 33, 0);
        }
    }
    // end_terminal(&shell->config);
    // exit(status);
	// shell->exit = 1;
	// shell->exit_status = status;
    //ft_free_arr(shell->env);
    (status && !tstatus) ? ft_putstr_fd("exit\n", 1) : 0;
    (!status && !tstatus) ? ft_putstr_fd("exit\n", 1) : 0;
    exit(status);
    return (0);
}