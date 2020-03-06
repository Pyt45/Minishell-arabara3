#include "../includes/shell.h"


void save_restor_fd(int save, int restore)
{
	static int fds[3];
	int i;

	i = -1;
	if(save)
	{
		while (++i < 3)
			fds[i] = dup(i);
	}
	if(restore)
	{
		while (++i < 3)
			dup2(fds[i], i);
	}
}

int     exec_commands(t_shell *shell, t_cmds *cmds)
{
	if (!cmds->cmd)
		return (1);
    if (!ft_strcmp(cmds->cmd, "env"))
        return (env_builtin(cmds, shell->env) * 0 + 1);
	else if (!ft_strcmp(cmds->cmd, "cd"))
        return (cd_builtin(shell, cmds)* 0 + 1);
    else if (!ft_strcmp(cmds->cmd, "pwd"))
        return (pwd_builtin()* 0 + 1);
    else if (!ft_strcmp(cmds->cmd, "exit"))
        return (exit_builtin(shell, cmds)* 0 + 1);
    else if (!ft_strcmp(cmds->cmd, "export"))
        return (export_builtin(shell, cmds)* 0 + 1);
    else if (!ft_strcmp(cmds->cmd, "unset"))
        return (unset_builtin(shell, cmds)* 0 + 1);
    else if (!ft_strcmp(cmds->cmd, "echo"))
       return (echo_builtin(cmds, shell->env));
    return (0);
}

// env


t_cmds     *excute_command_by_order(t_shell *shell, t_cmds *cmds, int num_pipe)
{
	// t_cmds	*cmds;
	pid_t	pid;
	int 	status = 0;
    int		i = 0;
	int		fds[2 * num_pipe];
	int		j = 0;
	
	//origina cmds
	// cmds = o_cmds;
	// printf("NUM PIPE %d\n----------------------\n", num_pipe);
	if ((cmds->next && !cmds->end) || !exec_commands(shell, cmds))
	{
		for (i = 0; i < num_pipe; i++)
		{
			if (pipe(fds + i * 2) < 0)
			{
				perror("couldn't pipe");
            	exit(EXIT_FAILURE);
			}
		}
		j = 0;
		while (cmds)
		{
			
			pid = fork();
			if (pid == 0)
			{
				// printf("1 S=%d E=%d |CMD=%s\n", cmds->start, cmds->end, cmds->cmd);
				// child gets input from the previous command, if it's not the first command
				if (cmds->next)
				{
					// printf("1.5 S=%d E=%d |CMD=%s\n", cmds->start, cmds->end, cmds->cmd);
					
					if (dup2(fds[j + 1], 1) < 0)
					{
						perror("1.|dup2|");
						exit(EXIT_FAILURE);
					}
				}
				// dprintf(2,"1.8 S=%d E=%d |CMD=%s\n", cmds->start, cmds->end, cmds->cmd);
				//child outputs to next command, if it's not the last command
				if (j != 0)
				{
					if (dup2(fds[j - 2], 0) < 0)
					{
						perror("2.|dup2|");
					}
				}
				for (i = 0; i < 2 *  num_pipe; i++)
					close(fds[i]);
				// 1 : > | 2 : >> | -1 : <
				// printf("2 S=%d E=%d |CMD=%s\n", cmds->start, cmds->end, cmds->cmd);
				char *s = get_bin_path(cmds->cmd, shell->env);
				// printf("%s | %s | %s\n", s, cmds->args[0], cmds->args[1]);
				// if (exec_commands(shell, cmds))
				// 	exit(0);
				if (execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)
				{
					perror("cmd");
					exit(EXIT_FAILURE);
				}
			}
			else if (pid < 0)
			{
				perror("Error");
				exit(EXIT_FAILURE);
			}
			if (cmds->end)
				break;
			else
				cmds = cmds->next;
			j += 2;
		}
		for (i = 0; i < 2 * num_pipe; i++)
			close(fds[i]);
	/* 	i = -1;
		while (++i < num_pipe)
			wait(&status); */
		while (waitpid(pid, &status, 0) < 0);
	}
    return (cmds);
}

int		get_num_pipes(t_cmds *cmds)
{
	int	i;

	i = 0;
	while (cmds->p)
	{
		i++;
		cmds = cmds->next;
	}
	return (i);
}

int		run_commands(t_shell *shell)
{
	t_cmds	*cmds;

	shell = parse_commands(shell);
	cmds = shell->cmds;
	while (cmds)
	{
		save_restor_fd(1,0);
		cmds = excute_command_by_order(shell, cmds, get_num_pipes(cmds));
		// dprintf(2, "\n---------before restoring fd\n");
		save_restor_fd(0,1);
		cmds = cmds->next;
		// printf("\n---------\n");
	}
	return (1);
}

// int main(int args, char **argv, char **env)
// {
// 	t_shell *shell;

// 	shell = malloc(sizeof(t_shell));
// 	shell->env = ft_arrdup(env);
// 	shell->line = ft_strdup("env | grep PATH ; env | grep PWD ; ls /bin | grep mkdir");
// 	execute_command(shell);

// }