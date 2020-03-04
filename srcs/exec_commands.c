#include "../includes/shell.h"

int     exec_commands(t_shell *shell)
{
    if (!ft_strcmp(shell->cmds->cmd, "env"))
        return (env_builtin(shell->cmds->args, shell->env));
    else if (!ft_strcmp(shell->cmds->cmd, "cd"))
        return (cd_builtin(shell));
    else if (!ft_strcmp(shell->cmds->cmd, "pwd"))
        return (pwd_builtin(shell));
    else if (!ft_strcmp(shell->cmds->cmd, "exit"))
        return (exit_builtin(shell));
    else if (!ft_strcmp(shell->cmds->cmd, "export"))
        return (export_builtin(shell));
    else if (!ft_strcmp(shell->cmds->cmd, "unset"))
        return (unset_builtin(shell));
    else if (!ft_strcmp(shell->cmds->cmd, "echo"))
        return (echo_builtin(shell->cmds, shell->env));
    return (0);
}

int     excute_command_by_order(t_shell *shell)
{
	pid_t	pid;
	int 	status;
    int		i = 0;
	int		fds[2 * shell->num_pipe];
	int		j = 0;
    shell = control_command(shell);
	
	if (shell->cmds->next || !exec_commands(shell))
	{
		for (i = 0; i < shell->num_pipe; i++)
		{
			if (pipe(fds + i * 2) < 0)
			{
				perror("couldn't pipe");
            	exit(EXIT_FAILURE);
			}
		}
		j = 0;
		while (shell->cmds)
		{
		
				pid = fork();
				if (pid == 0)
				{
					// child gets input from the previous command, if it's not the first command
					if (shell->cmds->next)
					{
						if (dup2(fds[j + 1], 1) < 0)
						{
							perror("1.|dup2|");
							exit(EXIT_FAILURE);
						}
					}
					//child outputs to next command, if it's not the last command
					if (j != 0)
					{
						if (dup2(fds[j - 2], 0) < 0)
						{
							perror("2.|dup2|");
						}
					}
					for (i = 0; i < 2 * shell->num_pipe; i++)
						close(fds[i]);
					if (execve(get_bin_path(shell->cmds->cmd, shell->env), shell->cmds->args, shell->env) < 0)
					{
						perror("cmd");
						exit(EXIT_FAILURE);
					}
				}
			/* else if (pid < 0)
			{
				perror("Error");
				exit(EXIT_FAILURE);
			} */
			if (shell->cmds)
				shell->cmds = shell->cmds->next;
			j += 2;
		}
	for (i = 0; i < 2 * shell->num_pipe; i++)
	{
		close(fds[i]);
	}
	//for (i = 0; i < shell->num_pipe + 1; i++)
	//{
		waitpid(pid, &status, 0);
		//wait(&status);
	//}
		/* if ((pid = fork()) == 0)
		{
			if(execve(get_bin_path(shell->cmds->cmd, shell->env), shell->cmds->args, shell->env) < 0)
				exit (EXIT_FAILURE);
			exit (EXIT_SUCCESS);
		}
		waitpid(pid , &status, 0); */
	}
	//shell->num_pipe = 0;
    return (1);
}