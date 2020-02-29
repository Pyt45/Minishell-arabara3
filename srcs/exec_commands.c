#include "../includes/shell.h"

int     exec_commands(t_shell *shell)
{
    if (!ft_strcmp(shell->cmds->cmd, "env"))
        return (env_builtin(shell->cmds->args, shell->env));
    else if (!ft_strcmp(shell->cmds->cmd, "cd"))
        return (cd_builtin(shell));
    /*if (!ft_strcmp(shell->cmds->cmd, "pwd"))
        return (pwd_builtin(shell));*/
    else if (!ft_strcmp(shell->cmds->cmd, "exit"))
        return (exit_builtin(shell));
   /*  if (!ft_strcmp(shell->cmds->cmd, "export"))
        return (export_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "unset"))
        return (unset_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "echo"))
        return (echo_builtin(shell)); */
    return (0);
}

int     excute_command_by_order(t_shell *shell)
{
	pid_t	pid;
	int status = 0;
    shell = control_command(shell);
	
	if (!exec_commands(shell))
	{
		if ((pid = fork()) == 0)
		{
			if(execve(get_bin_path(shell->cmds->cmd, shell->env), shell->cmds->args, shell->env) < 0)
				exit (EXIT_FAILURE);
			exit (EXIT_SUCCESS);
		}
		waitpid(pid , &status, 0);
	}
    return (1);
}