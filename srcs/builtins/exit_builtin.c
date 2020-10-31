#include "../../includes/shell.h"

int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
	double	tstatus;
	int		status;
	int		i;

	tstatus = 0;
	status = 0;
	i = -1;
	if (cmds->args[1])
	{
		while (cmds->args[1][++i])
		{
			if (ft_isalpha((int)(cmds->args[1][i])))
			{
				tstatus = 1;
				break ;
			}
		}
		status = ft_atoi(cmds->args[1]);
	}
	free_shell(shell);
	ft_free_arr(shell->env);
	ft_putstr_fd("exit\n", 1);
	(tstatus && !status) ? print_error("exit", 33, 0) : 0;
	exit(status);
	return (0);
}
