#include "../includes/shell.h"

int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
    int     status;

	status = 0;
	if (cmds->args[1] != NULL)
        status = ft_atoi(cmds->args[1]);
    exit(status);
    //ft_free_arr(shell->env);
    return (1);
}