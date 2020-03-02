
#include "../includes/shell.h"

int     export_builtin(t_shell *shell)
{
	int		i;

	i = 1;
	while (shell->cmds->args[i] != NULL)
	{
		shell->env = ft_export_cmd(shell, shell->cmds->args[i]);
		i++;
	}
	return (1);
}

char	**ft_export_cmd(t_shell *shell, char *value)
{	
	int		i;
	int		len;
	char	*record;
	char	**argv;

	argv = ft_split(value, '=');
	i = 0;
	if ((i = ft_getenv(argv[0], shell->env)) >= 0)
	{
		free(shell->env[i]);
		shell->env[i] = value;
	}
	else
		return (ft_add_to_arr(value, shell->env));
	return (shell->env);
}