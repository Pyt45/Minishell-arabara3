/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:35:31 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/29 19:38:23 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int		ft_getenv(char *name, char **env)
{
	char	*search;
	int		i;
	int		len;

	len = ft_strlen(name) + 2;
	if (!(search = (char *)malloc(sizeof(char) * len)))
		return (-1);
	ft_strcpy(search, name);
	ft_strcat(search, "=");
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], search, ft_strlen(name)) == 0)
		{
			ft_del(search);
			return (i);
		}
		i++;
	}
	ft_del(search);
	return (-1);
}

char	*try_path(char *filename, char *dir)
{
	char	*path;
	int		len;

	len = ft_strlen(filename) + ft_strlen(dir) + 2;
	if (!(path = (char *)malloc(sizeof(char) * len)))
		return (NULL);
	ft_strcpy(path, dir);
	ft_strcat(path, "/");
	ft_strcat(path, filename);
	if (ft_access(path, 1) == 1)
		return (path);
	return (NULL);
}

char	*get_bin_path(char *filename, char **env)
{
	char	*path;
	int		i;
	char	**a_path;
	char	*b_path;

	path = NULL;
	i = 0;
	if ((i = ft_getenv("PATH", env)) >= 0)
		path = env[i] + 5;
	if (path != NULL && (a_path = ft_split(path, ':')) != NULL)
	{
		i = 0;
		while (a_path[i] != NULL)
		{
			if ((b_path = try_path(filename, a_path[i])) != NULL)
			{
				ft_free_arr(a_path);
				return (b_path);
			}
			i++;
		}
	}
	return (filename);
}

int     ft_access(char *path, int mode)
{
    struct stat fileStat;

    if(stat(path, &fileStat) < 0)
        return (0);
    if (mode == 1)
        return (fileStat.st_mode & S_IXUSR ? 1 : 0);
    else if (mode == 2)
        return (fileStat.st_mode & S_IWUSR ? 1 : 0);
    else if (mode == 4)
        return (fileStat.st_mode & S_IRUSR ? 1 : 0);
    return (0);
}
