<<<<<<< HEAD
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 18:56:50 by aaqlzim           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2020/02/28 21:26:44 by aaqlzim          ###   ########.fr       */
=======
/*   Updated: 2020/02/28 20:55:10 by zlayine          ###   ########.fr       */
>>>>>>> 35891a2cd1ad892594ad167713b9e10e38fb488b
/*                                                                            */
/* ************************************************************************** */

>>>>>>> 7783d754a41fa85bd4c3bac1ba68873cc59f8da0

#include "../includes/shell.h"

int     ft_getenv(char *name, char **env)
{
    char    *search;
    int     i;
    int     len;

    len = ft_strlen(name) + 2;
    if (!(search = (char *)malloc(sizeof(char) * len)))
        return (-1);
    ft_strcpy(search, name);
    ft_strcat(search, "=");
    i = 0;
    while (env[i] != NULL)
    {
        if (ft_strncmp(env[i], search, 5) == 0)
        {
            free(search);
            return (i);
        }
        i++;
    }
    (search) ? (free(search)) : 0;
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
	free(path);
	return (NULL);
}

char    *get_bin_path(char *filename, char **env)
{
    char    *path;
    int     i;
    char    **a_path;
    char    *b_path;
    
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
				//ft_free_arr(a_path);
				return (b_path);
			}
			i++;
		}
	}
	return (filename);
}