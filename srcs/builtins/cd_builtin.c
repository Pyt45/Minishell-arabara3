#include "../../includes/shell.h"


int		move_to_dir(char *path, t_shell *shell, int *is_print)
{
	if (!path && *is_print)
		return (0);
	if (!path && (path = get_home_dir(shell)) != NULL && chdir(path))
		return (0);
	if (chdir(path))
		return (0);
	return (1);
}

char	*manage_path_cd(t_shell *shell, char *path, int *is_print)
{
	if (!ft_strcmp(path, "~"))
		path = get_home_dir(shell);
	else if (path[0] == '~' && path[1] == '/')
		path = ft_strcat(get_home_dir(shell), path + 1);
	else if (!ft_strcmp(path, "-"))
	{
		path = get_old_dir(shell);
		*is_print = 1;
	}
	return (path);
}

int     cd_builtin(t_shell *shell, t_cmds *cmds)
{
	char	*path;
	char	*pwd;
	int		is_print;
	int		ret;

	ret = 0;
	path = NULL;
	is_print = 0;
	pwd = getcwd(NULL, 0);
	if (cmds->args[1])
		path = manage_path_cd(shell, cmds->args[1], &is_print);
	ret = move_to_dir(path, shell, &is_print);
	(ret && is_print) ? ft_putendl_fd(path, 1) : 0;
	if (ret == 0)
		print_error(path, errno, 0);
	shell->env = ft_setenv("OLDPWD", pwd, shell->env);
	pwd = getcwd(NULL, 0);
	shell->env = ft_setenv("PWD", pwd, shell->env);
	return (!ret);
}
