#include "../includes/shell.h"

// int     exec_commands(t_shell *shell)
// {
//    /*  if (!ft_strcmp(shell->cmds->cmd, "env"))
//         return (env_builtin(shell->cmds->args, shell->env));
//     else  */if (!ft_strcmp(shell->cmds->cmd, "cd"))
//         return (cd_builtin(shell));
//     else if (!ft_strcmp(shell->cmds->cmd, "pwd"))
//         return (pwd_builtin(shell));
//     else if (!ft_strcmp(shell->cmds->cmd, "exit"))
//         return (exit_builtin(shell));
//     else if (!ft_strcmp(shell->cmds->cmd, "export"))
//         return (export_builtin(shell));
//     else if (!ft_strcmp(shell->cmds->cmd, "unset"))
//         return (unset_builtin(shell));
//     //else if (!ft_strcmp(shell->cmds->cmd, "echo"))
//     //    return (echo_builtin(shell->cmds, shell->env));
//     return (0);
// }

// env


int     excute_command_by_order(t_shell *shell, t_m_cmds *p_cmds, int num_pipe)
{
	t_m_cmds	*cmds;
	pid_t	pid;
	int 	status;
    int		i = 0;
	int		fds[2 * num_pipe];
	int		j = 0;
	
	cmds = p_cmds;
	if (cmds->next /* || !exec_commands(shell->env, cmds->s_cmds) */)
	{
		for (i = 0; i < 2 * num_pipe; i++)
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
				//child outputs to next command, if it's not the last command
				if (j != 0)
				{
					if (dup2(fds[(j - 1) * 2], 0) < 0)
					{
						perror("2.|dup2|");
					}
				}
				// child gets input from the previous command, if it's not the first command
				if (cmds->next)
				{
					if (dup2(fds[j * 2 + 1], 1) < 0)
					{
						perror("1.|dup2|");
						exit(EXIT_FAILURE);
					}
				}
				for (i = 0; i < 2 * num_pipe; i++)
					close(fds[i]);
				// 1 : > | 2 : >> | -1 : <
				if (cmds->s_cmds->append)
				if (/* !exec_commands(shell->env, cmds->s_cmds) ||  */execve(get_bin_path(cmds->s_cmds->cmd, shell->env), cmds->s_cmds->args, shell->env) < 0)
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
			else
			{
				if (cmds != NULL)
					cmds = cmds->next;
				j++;
				for (i = 0; i < 2 * num_pipe; i++)
					close(fds[i]);
				while (waitpid(pid, &status, 0) < 0);
			}
		}
	}
	//shell->num_pipe = 0;
    return (1);
}

int		execute_command(t_shell *shell)
{
	t_b_cmds	*cmds;

	shell = get_comma_commands(shell);
	cmds = shell->cmds;
	while (cmds)
	{
		excute_command_by_order(shell, cmds->m_cmds, cmds->num_pipe);
		cmds = cmds->next;
	}
	return (0);
}

/* int main(int args, char **argv, char **env)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	shell->env = ft_arrdup(env);
	shell->line = ft_strdup("env | grep PATH ; env | grep PWD ; ls /bin | grep mkdir");
	execute_command(shell);

} */