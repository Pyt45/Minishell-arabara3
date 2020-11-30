/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 18:41:47 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/28 18:23:03 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*get_dir(char **path)
{
	char	*tmp;

	if (ft_strchr(*path, '='))
	{
		tmp = ft_strdup(ft_strchr(*path, '=') + 1);
		ft_del(*path);
		*path = tmp;
		return (*path);
	}
	return (*path);
}

int		move_to_dir(char **path, int *is_print)
{
	if (!*path && *is_print == 1)
		return (0);
	if (!ft_strlen(*path))
		return (1);
	if (chdir(get_dir(path)))
	{
		ft_del(*path);
		return (0);
	}
	return (1);
}

char	*manage_path_cd(t_shell *shell, char *path, int *is_print)
{
	char	*tmp;

	if (!ft_strcmp(path, "~"))
		path = get_home_dir(shell);
	else if (path[0] == '~' && path[1] == '/')
	{
		tmp = get_home_dir(shell);
		path = ft_strjoin(tmp, path + 1);
		ft_del(tmp);
	}
	else if (!ft_strcmp(path, "-"))
	{
		path = get_old_dir(shell);
		*is_print = 1;
	}
	else
		return (ft_strdup(path));
	return (path);
}

void	set_pwd(t_shell *shell, char *pwd)
{
	shell->env = ft_setenv("OLDPWD", pwd, shell->env);
	ft_del(pwd);
	pwd = getcwd(NULL, 0);
	shell->env = ft_setenv("PWD", pwd, shell->env);
	ft_del(pwd);
}

int		cd_builtin(t_shell *shell, t_cmds *cmds)
{
	char	*path;
	char	*pwd;
	int		is_print;
	int		ret;

	ret = 0;
	path = NULL;
	is_print = 0;
	pwd = getcwd(NULL, 0);
	path = manage_path_cd(shell, cmds->args[1], &is_print);
	ret = move_to_dir(&path, &is_print);
	(ret && is_print) ? ft_putendl_fd(path, 1) : 0;
	if (ret == 0)
	{
		if (is_print == 1)
			print_error("OLDPWD not set", errno, 0);
		else if (!is_print)
			print_error(cmds->args[1], errno, 0);
		ft_del(pwd);
		return (!ret);
	}
	if (ret == 1 && is_print == 2)
	{
		print_error("cd: HOME not set", errno, 0);
		return (ret);
	}
	set_pwd(shell, pwd);
	ft_del(path);
	return (!ret);
}
