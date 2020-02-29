#include "../includes/shell.h"


int     env_builtin(char **argv, char **env)
{
    if (argv[1] != NULL)
        ft_putstr_fd("Error Env has no arg\n", 2);
	else
   		ft_print_env_arr(env);
    return (1);
}