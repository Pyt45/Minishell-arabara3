#include "../includes/shell.h"

int     cd_builtin(t_shell *shell)
{
	char	*path;
	//char	*path1;
	//int		i;

    if (shell->cmds->args[1] != NULL)
    {
		path = shell->cmds->args[1];
		if (!ft_strcmp(path, "~"))
		{
			if ((path = get_home_dir(shell)) != NULL && chdir(path) != 0)
				ft_putstr_fd("Error: file or dir not exist\n", 2);
		}
		else
			if (chdir(path) != 0)
				ft_putstr_fd("Error: file or dir not exist\n", 2);
    }
	else
	{
		if ((path = get_home_dir(shell)) != NULL && chdir(path) != 0)
			ft_putstr_fd("Error: file or dir not exist\n", 2);
	}
	/* if ((i = ft_getenv("PWD", shell->env)))
	{
		if (!(path1 = (char *)malloc(sizeof(char) * (ft_strlen(shell->env[i]) + ft_strlen(path) + 6))))
			return (-1);
		path1 = shell->env[i] + 4;
		ft_strcat(path1, "/");
		ft_strcat(path1, path);
	} */
	shell->env = ft_setenv("PWD", path, shell->env);
	return (1);
}