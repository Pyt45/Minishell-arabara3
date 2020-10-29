/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:11:38 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/29 19:28:32 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static char		**ft_sort_export(char **env)
{
	int		i;
	int		j;
	int		k;
	char	*tmp;
	int		len;

	i = -1;
	len = ft_arr_len(env);
	while (i < len)
	{
		j = i;
		while (++j < len)
		{
			k = 0;
			while (env[i][k] == env[j][k])
				k++;
			if (env[i][k] > env[j][k])
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
		}
	}
	return (env);
}

static char		*ft_get_first(const char *s, int c)
{
	char	r;
	int		i;

	r = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == r || s[i + 1] == '\0')
			return (ft_substr(s, 0, i + 1));
		i++;
	}
	if (r == '\0')
		return ((char *)s);
	return (0);
}

static void		ft_print_export(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i] != NULL)
		{
			ft_putstr_fd("decalre -x ", 1);
			ft_putstr_fd(ft_get_first(arr[i], '='), 1);
			if (ft_strchr(arr[i], '='))
			{
				ft_putchar_fd('"', 1);
				ft_putstr_fd(ft_strchr(arr[i], '=') + 1, 1);
				ft_putchar_fd('"', 1);
			}
			ft_putstr_fd("\n", 1);
			i++;
		}
	}
}

int				ft_export_cmd(t_shell *shell, char *value)
{
	int		i;
	char	**argv;

	i = 0;
	argv = ft_split(value, '=');
	if (value[0] == '=')
		return (0);
	if (argv[1] && (i = ft_getenv(argv[0], shell->env)) >= 0)
	{
		free(shell->env[i]);
		shell->env[i] = value;
	}
	else
		shell->env = ft_add_to_arr(value, shell->env);
	return (1);
}

int				export_builtin(t_shell *shell, t_cmds *cmds)
{
	int		i;
	char	**env_sort;
	char	**new_env;
	int		err;

	i = 1;
	err = 0;
	if (cmds->args[1] != NULL)
	{
		while (cmds->args[i])
		{
			if (ft_isdigit(cmds->args[i][0]) ||
				ft_strcmp(cmds->args[i], ' ') ||
				!ft_export_cmd(shell, cmds->args[i]))
				err = print_error("invalid identifier", errno, 0);
			i++;
		}
	}
	else
	{
		env_sort = ft_arrdup(shell->env);
		new_env = ft_sort_export(env_sort);
		ft_print_export(new_env);
	}
	return (err);
}
