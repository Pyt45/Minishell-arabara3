
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

static char **ft_malloc_arr(char **arr, int add)
{
	int		i;
	int		len;
	size_t	str_len;
	char	**new_env;

	len = ft_arr_len(arr);
	if (!(new_env = (char **)malloc(sizeof(char *) * (len + 1))))
		return (NULL);
	if (add)
	{
		i = 0;
		while (i < len)
		{
			str_len = ft_strlen(arr[i]);
			if (!(new_env[i] = (char *)malloc(sizeof(char) * (str_len + 1))))
				return (NULL);
			i++;
		}
		new_env[i] = NULL;
	}
	return (new_env);
}

static char	*ft_handle_quets(char *str,char *str1, size_t strlen)
{
	str[0] = '"';
	ft_strcpy(str + 1, str1);
	str[strlen - 2] = '"';
	str[strlen - 1] = '\0';
	return (str);
}

static char	*ft_get_first(const char *s, int c)
{
	char	r;
	int		i;

	r = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == r)
			return (ft_substr(s, 0, i + 1));
		i++;
	}
	if (r == '\0')
		return ((char *)s);
	return (0);
}

static char	**ft_export_join(char **env)
{
	char	**env_arr;
	char	**new_env;
	char	**used_arr;
	size_t	str_len;
	size_t	len;
	int		i = 0;

	if (env)
	{
		env_arr = ft_malloc_arr(env, 0);
		new_env = ft_malloc_arr(env, 1);
		used_arr = ft_malloc_arr(env, 0);
		while (env[i] != NULL)
		{
			if (ft_strchr(env[i], '='))
			{
				len = ft_strlen(ft_get_first(env[i], '='));
				str_len = ft_strlen(ft_strchr(env[i], '=') + 1);
				if (!(env_arr[i] = (char *)malloc(sizeof(char) * (str_len + 3))))
					return (NULL);
				env_arr[i] = ft_handle_quets(env_arr[i], ft_strchr(env[i], '=') + 1, str_len + 3);
				if (!(used_arr[i] = (char *)malloc(sizeof(char) * (len + 1))))
					return (NULL);
				used_arr[i] = ft_get_first(env[i], '=');
				new_env[i] = ft_strjoin(used_arr[i], env_arr[i]);
				//https://harm-smits.github.io/42docs/projects
			}
			i++;
		}
	}
	return (new_env);
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
            ft_putstr_fd(arr[i], 1);
			ft_putstr_fd("\n", 1);
        	i++;
		}
    }
}

int     export_builtin(t_shell *shell, t_cmds *cmds)
{
	int		i;
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
		new_env = ft_export_join(shell->env);
		new_env = ft_sort_export(new_env);
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
	if (ft_strchr(value, '='))
	{
		argv = ft_split(value, '=');
		if ((i = ft_getenv(argv[0], shell->env)) >= 0)
		{
			free(shell->env[i]);
			shell->env[i] = value;
		}
		else
			return (ft_add_to_arr(value, shell->env));
	}
	return (shell->env);
}