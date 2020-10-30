/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:31:48 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/29 19:33:08 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*get_home_dir(t_shell *shell)
{
	int		i;

	i = 0;
	if ((i = ft_getenv("HOME", shell->env)) >= 0)
		return (ft_strdup(shell->env[i] + 5));
	return (NULL);
}

char	*get_old_dir(t_shell *shell)
{
	int		i;

	i = 0;
	if ((i = ft_getenv("OLDPWD", shell->env)) >= 0)
		return (shell->env[i] + 7);
	return (NULL);
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
	}
	else
		return (ft_add_to_arr(record, env));
	// do not remove //
	//ft_del(record);
	return (env);	
}