#include "../../includes/shell.h"

int     cd_builtin(t_shell *shell, t_cmds *cmds)
{
	char	*path;
	char	*home_dir;
	char	*new_pwd;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
    if (cmds->args[1] != NULL)
    {
		path = cmds->args[1];
		if (!ft_strcmp(path, "~"))
		{
			if ((path = get_home_dir(shell)) != NULL && chdir(path) != 0)
				ft_putstr_fd("Error: file or dir not exist\n", 2);
		}
		else
		{
			if (path[0] == '~' && path[1] == '/')
			{
				home_dir = ft_strdup(get_home_dir(shell));
				path = ft_strcat(home_dir, path + 1);
				if (path != NULL && chdir(path) != 0)
					ft_putstr_fd("Error: file or dir not exist\n", 2);
			}
			else
			{
				if (chdir(path) != 0)
					ft_putstr_fd("Error: file or dir not exist\n", 2);
			}
		}
    }
	else
	{
		if ((path = get_home_dir(shell)) != NULL && chdir(path) != 0)
			ft_putstr_fd("Error: file or dir not exist\n", 2);
	}
	shell->env = ft_setenv("OLDPWD", old_pwd, shell->env);
	new_pwd = getcwd(NULL, 0);
	shell->env = ft_setenv("PWD", new_pwd, shell->env);
	return (1);
}