
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

char    **ft_get_arr(char *value, char **arr)
{
    int     i;
    char    **new_arr;
    int     len;

    i = 0;
    len = ft_arr_len(arr) + 2;
    if (!(new_arr = (char **)malloc(sizeof(char*) * len)))
        return (NULL);
    while (arr[i] != NULL)
    {
		new_arr[i] = malloc(sizeof(char) * (ft_strlen(arr[i]) + 1));
        new_arr[i] = arr[i];
        i++;
    }
    //free(arr);
    new_arr[i] = value;
    new_arr[i + 1] = NULL;
    return (new_arr);
}

char    **ft_add_to_arr(char *value, char **arr)
{
    char    **new_arr;
    if (arr == NULL)
    {
        if (!(new_arr = (char **)malloc(sizeof(char *) * 2)))
            return (NULL);
        new_arr[0] = value;
        new_arr[1] = NULL;
    }
    else
        return (ft_get_arr(value, arr));
    return (new_arr);
}

char	**ft_copy_arr_without(int pos, char **arr, char **new_arr, int len)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (i != pos)
		{
			if (!(new_arr[j] = (char *)malloc(sizeof(char) * (ft_strlen(arr[i]) + 1))))
				return (NULL);
			ft_strcpy(new_arr[j], arr[i]);
			j++;
		}
		i++;
	}
	new_arr[j] = NULL;
	return (new_arr);
}

char    **ft_remove_from_arr(int pos, char **arr)
{
	int		i;
	int		len;
	char	**new_arr;

	if (arr != NULL)
	{
		len = ft_arr_len(arr);
		if (!(new_arr = (char**)malloc(sizeof(char *) * len)))
			return (NULL);
		new_arr = ft_copy_arr_without(pos, arr, new_arr, len);
		ft_free_arr(arr);
	}
	return (new_arr);
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

int     get_error_num(int err)
{
    if (err == 13)
        return (126);
    if (err == 2)
        return (127);
    return (1);
}

double  ft_fatoi(const char *str)
{
    int		i;
	double	n;
	int		sign;
    int     point;

	i = 0;
	n = 0;
    point = 0;
	sign = 1;
	while (str[i] == '\t' || str[i] == ' ' || str[i] == '\n' ||
		str[i] == '\v' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] != '\0' && (ft_isdigit(str[i]) || (str[i] == '.' && point == 0)))
	{
        if (ft_isdigit(str[i]))
		    n = n * 10 + (str[i] - 48);
        else
            point = i;
		i++;
	}
    if (point)
        n = n / ft_pow(10, i - point);
	n = n * sign;
	return (n);
}
//      ---------------- TEST   ------------------

// int main()
// {
//     printf("%d\n", ft_access("/bin/find", 1));
// }

