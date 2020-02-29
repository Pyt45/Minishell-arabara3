#include "../includes/shell.h"

int		exit_builtin(t_shell *shell)
{
    int     status;

	status = 0;
	if (shell->cmds->args[1] != NULL)
        status = ft_atoi(shell->cmds->args[1]);
    exit(status);
    //ft_free_arr(shell->env);
    return (1);
}