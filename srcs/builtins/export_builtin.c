
#include "../../includes/shell.h"


int     export_builtin(t_shell *shell, t_cmds *cmds)
{
	int		i;

	i = 1;
	while (cmds->args[i])
	{
		shell->env = ft_export_cmd(shell, cmds->args[i]);
		i++;
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