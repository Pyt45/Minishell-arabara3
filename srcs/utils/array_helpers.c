/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 17:01:04 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/11 09:41:06 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**ft_get_arr(char *value, char **arr)
{
	int		i;
	char	**new_arr;
	int		len;

	i = -1;
	new_arr = NULL;
	len = ft_arr_len(arr) + 2;
	if (!(new_arr = (char **)malloc(sizeof(char*) * len)))
		return (NULL);
	while (arr[++i] != NULL)
		new_arr[i] = ft_strdup(arr[i]);
	ft_free_arr(arr);
	new_arr[i] = ft_strdup(value);
	new_arr[i + 1] = NULL;
	ft_del(value);
	return (new_arr);
}

char	**ft_add_to_arr(char *value, char **arr)
{
	char	**new_arr;

	if (arr == NULL)
	{
		if (!(new_arr = (char **)malloc(sizeof(char *) * 2)))
			return (NULL);
		new_arr[0] = ft_strdup(value);
		new_arr[1] = NULL;
		return (new_arr);
	}
	else
		return (ft_get_arr(value, arr));
}

char	**ft_remove_from_arr(int pos, char **arr)
{
	int		len;
	char	**new_arr;

	new_arr = NULL;
	if (arr)
	{
		len = ft_arr_len(arr);
		if (!(new_arr = (char**)malloc(sizeof(char *) * len)))
			return (NULL);
		new_arr = ft_copy_arr_without(pos, arr, new_arr, len);
		ft_free_arr(arr);
	}
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
			if (!(new_arr[j] = (char *)malloc(sizeof(char) *
				(ft_strlen(arr[i]) + 1))))
				return (NULL);
			ft_strcpy(new_arr[j], arr[i]);
			j++;
		}
		i++;
	}
	new_arr[j] = NULL;
	return (new_arr);
}
