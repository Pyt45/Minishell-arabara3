#include "../../includes/shell.h"


int     unset_builtin(t_shell *shell, t_cmds *cmds)
{
    int     i;

    i = 1;
    while (cmds->args[i] != NULL)
    {
        shell->env = ft_unset_cmd(shell, cmds->args[i]);
        i++;
    }
    return (1);
}

char    **ft_unset_cmd(t_shell *shell, char *value)
{
	int		i;

	i = 0;
	if ((i = ft_getenv(value, shell->env)) >= 0)
		shell->env = ft_remove_from_arr(i, shell->env);
	return (shell->env);
}