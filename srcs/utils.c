
#include "../includes/shell.h"

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
