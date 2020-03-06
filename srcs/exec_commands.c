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

int		*pipe_fds(int num_pipe, int *fds)
{
	int i;

	i = 0;
	fds = malloc(sizeof(int) * num_pipe);
	while (i < num_pipe)
	{
		if (pipe(fds + i * 2) < 0)
		{
			perror("couldn't pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (fds);
}

int		*create_fds(t_cmds *cmds, int j, int *fds)
{
	if (cmds->next)
	{
		if (dup2(fds[j + 1], 1) < 0)
		{
			perror("1.|dup2|");
			exit(EXIT_FAILURE);
		}
	}
	if (j != 0)
	{
		if (dup2(fds[j - 2], 0) < 0)
		{
			perror("2.|dup2|");
		}
	}
	return (fds);
}

t_cmds     *excute_command_by_order(t_shell *shell, t_cmds *cmds, int num_pipe)
{
	pid_t	pid;
	int 	status = 0;
    int		i = 0;
	int		*fds;
	int		j = 0;
	
	if ((cmds->next && !cmds->end) || !exec_commands(shell, cmds))
	{
		fds = pipe_fds(num_pipe, fds);
		j = 0;
		while (cmds)
		{
			pid = fork();
			if (pid == 0)
			{
				fds = create_fds(cmds, j, fds);
				for (i = 0; i < 2 *  num_pipe; i++)
					close(fds[i]);
				if (!exec_commands(shell, cmds) && execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)
				{
					perror("cmd");
					exit(EXIT_FAILURE);
				}
				dprintf(2, "CMD=%s\n", cmds->cmd);
				printf("CMD=%s\n", cmds->cmd);
			}
			// else if (pid < 0)
			// {
			// 	perror("Error");
			// 	exit(EXIT_FAILURE);
			// }
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
		free(fds);
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
		save_restor_fd(0,1);
		cmds = cmds->next;
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