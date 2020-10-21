
#include "../../includes/shell.h"


static char	**ft_sort_export(char **env)
{
	int 	i;
	int		j;
	char	*tmp;

	i = 0;
	if (env)
	{
		while (i < ft_arr_len(env))
		{
			j = i + 1;
			while (j < ft_arr_len(env))
			{
				if (env[i][0] > env[j][0])
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

static void ft_export_join(char **env)
{
	char	**new_env;
	int		i = 0;
	//int		j = 0;

	if (env)
	{
		while (env[i] != NULL)
		{
			if (ft_strchr(env[i], '='))
			{
				new_env = ft_split(env[i], '=');
				printf("|%s|\n", new_env[i] + 1);
			}
			i++;
		}
		//ft_print_env_arr(new_env);
	}
}

int     export_builtin(t_shell *shell, t_cmds *cmds)
{
	int		i;
	char	**new_env;

	i = 1;
	/* while (cmds->args[i])
	{
		shell->env = ft_export_cmd(shell, cmds->args[i]);
		i++;
	} */
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
		//new_env = ft_sort_export(shell->env);
		//ft_print_env_arr(new_env);
		 ft_export_join(shell->env);
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