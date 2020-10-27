#include "../../includes/shell.h"

// int     cd_builtin(t_shell *shell, t_cmds *cmds)
// {
// 	char	*path;
// 	char	*home_dir;
// 	char	*new_pwd;
// 	char	*old_pwd;

// 	old_pwd = getcwd(NULL, 0);
//     if (cmds->args[1] != NULL)
//     {
// 		path = cmds->args[1];
// 		if (!ft_strcmp(path, "~"))
// 		{
// 			if ((path = get_home_dir(shell)) != NULL && chdir(path) != 0)
// 				print_error(path, 2, 0);
// 		}
// 		else
// 		{
// 			if (path[0] == '~' && path[1] == '/')
// 			{
// 				home_dir = ft_strdup(get_home_dir(shell));
// 				path = ft_strcat(home_dir, path + 1);
// 				if (path != NULL && chdir(path) != 0)
// 					print_error(path, 2, 0);
// 			}
// 			else
// 			{
// 				if (chdir(path) != 0)
// 					print_error(path, 2, 0);
// 			}
// 		}
//     }
// 	else
// 	{
// 		if ((path = get_home_dir(shell)) != NULL && chdir(path) != 0)
// 			print_error(path, 2, 0);
// 	}
// 	shell->env = ft_setenv("OLDPWD", old_pwd, shell->env);
// 	new_pwd = getcwd(NULL, 0);
// 	shell->env = ft_setenv("PWD", new_pwd, shell->env);
// 	return (1);
// }


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
	if (cmds->args[1] != NULL)
	{
		path = cmds->args[1];
		if (!ft_strcmp(path, "~"))
			path = get_home_dir(shell);
		else if (path[0] == '~' && path[1] == '/')
			path = ft_strcat(get_home_dir(shell), path + 1);
		else if (!ft_strcmp(path, "-"))
		{
			path = get_old_dir(shell);
			is_print = 1;
		}
	}
	ret = move_to_dir(path, shell, &is_print);
	(ret && is_print) ? ft_putendl_fd(path, 1) : 0;
	if (ret == 0)
		print_error(path, 2, 0);
	shell->env = ft_setenv("OLDPWD", pwd, shell->env);
	pwd = getcwd(NULL, 0);
	shell->env = ft_setenv("PWD", pwd, shell->env);
	shell->ret = !ret;
	return (ret);
}
