
#include "../../includes/shell.h"


static char	**ft_sort_export(char **env)
{
	int 	i;
	int		j;
	int		k;
	char	*tmp;

	i = 0;
	if (env)
	{
		env = ft_arrdup(env);
		while (i < ft_arr_len(env))
		{
			j = i + 1;
			while (j < ft_arr_len(env))
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
				j++;
			}
			i++;
		}
	}
	return (env);
}


static char	*ft_get_first(const char *s, int c)
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

static void    ft_print_export(char **arr)
{
    int     i;

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

int     export_builtin(t_shell *shell, t_cmds *cmds)
{
	int		i;
	char	**env_sort;
	char	**new_env;

	i = 1;
	if (cmds->args[1] != NULL)
	{
		while (cmds->args[i])
		{
			shell->env = ft_export_cmd(shell, cmds->args[i]);
			i++;
		}
	}
	else
	{
		env_sort = ft_arrdup(shell->env);
		new_env = ft_sort_export(env_sort);
		ft_print_export(new_env);
	}
	return (0);
}

char	**ft_export_cmd(t_shell *shell, char *value)
{	
	int		i;
	int		len;
	char	*record;
	char	**argv;

	i = 0;
	argv = ft_split(value, '=');
	if (argv[1] && (i = ft_getenv(argv[0], shell->env)) >= 0)
	{
		free(shell->env[i]);
		shell->env[i] = value;
	}
	else
		return (ft_add_to_arr(value, shell->env));
	return (shell->env);
}