
#include "../includes/shell.h"

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

    i = -1;
    len = ft_arr_len(arr) + 2;
    if (!(new_arr = (char **)malloc(sizeof(char*) * len)))
        return (NULL);
    while (arr[++i] != NULL)
		new_arr[i] = ft_strdup(arr[i]);
	ft_free_arr(arr);
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
    	return (new_arr);
    }
    else
        return (ft_get_arr(value, arr));
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

// int     get_special_char(char c)
// {
//     if (c == 'n')
//         return (10);
//     else if (c == 'r')
//         return (13);
//     else if (c == 'v')
//         return (11);
//     else if (c == 't')
//         return (9);
//     else if (c == 'f')
//         return (12);
//     else
//         return (c);
// }
