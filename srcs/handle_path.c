
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
        if (ft_strncmp(env[i], search, ft_strlen(name)) == 0)
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

char	**ft_setenv(char *var, char *path, char **env)
{
	int		i;
	int		len;
	char	*record;
	
	len = ft_strlen(var) + ft_strlen(path) + 2;
	if (!(record = (char *)malloc(sizeof(char) * len)))
		return (NULL);
	ft_strcpy(record, var);
	ft_strcat(record, "=");
	ft_strcat(record, path);
	if ((i = ft_getenv(var, env)) >= 0)
	{
		free(env[i]);
		env[i] = record;
		//printf("%s\n", env[i]);
	}
	/* else
		return (ft_add_to_arr()); */
	return (env);	
}

char	*get_home_dir(t_shell *shell)
{
	int		i;

	i = 0;
	if ((i = ft_getenv("HOME", shell->env)) >= 0)
		return (shell->env[i] + 5);
	return (NULL);
}

char	*get_old_dir(t_shell *shell)
{
	int		i;

	i = 0;
	if ((i = ft_getenv("OLDPWD", shell->env)) >= 0)
		return (shell->env[i] + 6);
	return (NULL);
}