#include "../../includes/shell.h"



int     env_builtin(t_cmds *cmd, char **env)
{
    if (cmd->args[1] != NULL)
    {
        ft_putstr_fd("env: ", 2);
        ft_putstr_fd(cmd->args[1], 2);
        ft_putstr_fd(": No such file or directory\n", 2);
    }
	else
   		ft_print_env_arr(env);
    return (1);
}