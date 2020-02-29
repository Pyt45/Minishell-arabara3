#include "../includes/shell.h"

int     cd_builtin(t_shell *shell)
{
	char	*path;

    if (shell->cmds->args[1] != NULL)
    {
		path = shell->cmds->args[1];
		if (chdir(path) != 0)
			ft_putstr_fd("Error: file or dir not exist\n", 2);
    }
	else
	{
		if ((path = get_home_dir(shell->env)) != NULL && chdir(path) != 0)
			ft_putstr_fd("Error: file or dir not exist\n", 2);
	}
	//shell->env = ft_setenv("PWD", path, shell->env);
	return (1);
}