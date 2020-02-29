
#include "../includes/shell.h"

char    *ft_strcat(char *dest, char *src)
{
    int     i;
    int     len;
    i = 0;
    len = ft_strlen(dest);
    while (dest[i] != '\0')
    {
        dest[i + len] = src[i];
        i++;
    }
    dest[i + len] = '\0';
    return (dest);
}

char    *ft_strcpy(char *s1, char *s2)
{
    int     i;

    i = 0;
    while (s2[i] != '\0')
    {
        s1[i] = s2[i];
        i++;
    }
    s1[i] = '\0';
    return (s1);
}

int     ft_arr_len(char **arr)
{
    int     i;

    i = 0;
    if (arr)
    {
        while (arr[i] != NULL)
            i++;
    }
    return (i);
}

char    **ft_arrdup(char **arr)
{
    int     i;
    int     len;
	size_t	str_len;
    char    **new_arr;

    len = ft_arr_len(arr);
    if (!(new_arr = (char **)malloc(sizeof(char *) * (len + 1))))
        return (NULL);
    i = 0;
    while (i < len)
    {
        str_len = (int)ft_strlen(arr[i]);
        if (!(new_arr[i] = (char *)malloc(sizeof(char) * (str_len + 1))))
            return (NULL);
        ft_strcpy(new_arr[i], arr[i]);
        i++;
    }
    new_arr[i] = NULL;
    return (new_arr);
}

void    ft_print_env_arr(char **arr)
{
    int     i;

    i = 0;
    if (arr)
    {
        while (arr[i] != NULL)
		{
            ft_putstr_fd(arr[i], 1);
			ft_putstr_fd("\n", 1);
        	i++;
		}
    }
}

int		ft_free_arr(char **arr)
{
	int		i;

	i = 0;
	if (!arr)
		return (1);
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	(arr) ? (free(arr)) : 0;
	return (1);
}

// MODES: 1:X 2:W 4:R
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

//      ---------------- TEST   ------------------

// int main()
// {
//     printf("%d\n", ft_access("/bin/find", 1));
// }