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
       return (echo_builtin(cmds, shell->env, shell->ret));
    return (0);
}

int		*pipe_fds(int num_pipe, int *fds)
{
	int i;

	i = 0;
	fds = malloc(sizeof(int) * 2 * num_pipe);
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
	if (j != 0)
	{
		if (dup2(fds[j - 2], 0) < 0)
		{
			perror("2.|dup2|");
			exit(EXIT_FAILURE);
		}
	}
	if (cmds->next)
	{
		if (dup2(fds[j + 1], 1) < 0)
		{
			perror("1.|dup2|");
			exit(EXIT_FAILURE);
		}
	}
	return (fds);
}

int		*create_ior(t_cmds *cmd, int j, int *fds)
{
	if (j != 0)
	{
		if (dup2(fds[j - 2], 0) < 0)
		{
			perror("3.|dup2|");
			exit(EXIT_FAILURE);
		}
	}
	if (cmd->next)
	{
		if (dup2(fds[j + 1], 1) < 0)
		{
			perror("4.|dup2|");
			exit(EXIT_FAILURE);
		}
	}
	return (fds);
}

int		*pipe_ior(int num_sp, int *ior)
{
	int i;

	i = 0;
	ior = malloc(sizeof(int) * (2 * num_sp));
	while (i < num_sp)
	{
		if (pipe(ior + i * 2) < 0)
		{
			perror("couldn't pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (ior);
}


int			open_output(t_cmds *cmd, int append, int ofd)
{
	int		fd;
	int		flag;

	flag = O_WRONLY | O_CREAT;
	if (append)
		flag = flag | O_APPEND;
	else
		flag = flag | O_TRUNC;
	if ((fd = open(cmd->next->args[0], flag, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) >= 0)
		ofd = fd;
	dup2(ofd, 1);
	return (ofd);
}

void	read_from_stdin(int fd)
{
	char	buff[1024];
	int		r;

	while ((r = read(0, buff, 1024)))
	{
		if (write(fd, buff, r) < r)
			return ;
	}
}

int		open_input(char *args, int append, int ifd)
{
	int		fd;
	int		pipefd[2];
	
	if (append)
	{
		pipe(pipefd);
		read_from_stdin(pipefd[1]);
		close(pipefd[1]);
		ifd = pipefd[0];
	}
	else if ((fd = open(args, O_RDONLY)) >= 0)
	{
		//puts("here\n");
		ifd = fd;
	}
	dup2(ifd, 1);
	return (ifd);
}

void		do_redirect(t_cmds *cmd, int fd[2])
{
	if (cmd->append == -2) // <<
		fd[0] = open_input(cmd->args[1], 1, fd[0]);
	else if (cmd->append == 2) // >>
	{
		fd[1] = open_output(cmd, 1, fd[1]);
		//printf("%s %s\n", cmd->args[1], cmd->next->args[0]);
	}
	if (cmd->append == 1) // >
		fd[1] = open_output(cmd, 0, fd[1]);
	else if (cmd->append == -1) // <
		fd[1] = open_input(cmd->next->args[0], 0, fd[1]);
}

void		exec_io_redi(t_cmds *cmd, int ifd, int ofd, t_shell *shell)
{
	int		new_fd[2];

	new_fd[0] = ifd;
	new_fd[1] = ofd;

	do_redirect(cmd, new_fd);
	if (new_fd[0] != ifd)
		close(new_fd[0]);
	if (new_fd[1] != ofd)
		close(new_fd[1]);
}

t_cmds     *excute_command_by_order(t_shell *shell, t_cmds *cmds, int num_pipe, int num_sp)
{
	pid_t	pid;
	int 	status = 0;
	char	*line;
	char	*content;
	int		r = 1;
    int		i = 0;
	int		*ior;
	int		*fds;
	int		j = 0;
	
	//num_pipe = 1;
	(num_pipe) ? fds = pipe_fds(num_pipe, fds) : 0;
	(num_sp) ? fds = pipe_ior(num_sp, fds) : 0;
	if ((cmds->next && !cmds->end) || !exec_commands(shell, cmds))
	{
		//fds = pipe_fds(num_pipe, fds);
		j = 0;
		while (cmds)
		{
			save_restor_fd(1,0);
			pid = fork();
			if (pid == 0)
			{

				(num_pipe) ? fds = create_fds(cmds, j, fds) : 0;
				for (i = 0; i < 2 * num_pipe; i++)
					close(fds[i]);
				/*if (cmds->append)
				{
					if (!cmds->prev || cmds->prev->append == 0)
					{
						// run exec
					}
					else
					{
						// open fd , print f 1
					}

					// printf("CMD=%s ARG0=%s ARG1=%s\n", cmds->cmd, cmds->args[0], cmds->args[1]);
					// printf("CMD=%s ARG0=%s ARG1=%s\n", cmds->next->cmd, cmds->next->args[0], cmds->next->args[1]);
				}
				else if (cmds->prev && cmds->prev->append)
				{

					//printf("CMD=%s ARG1=%s ARG2=%s\n", cmds->cmd, cmds->args[0], cmds->args[1]);
				}
				else
				{
					char *s = get_bin_path(cmds->cmd, shell->env);
					// printf("PATH: %s\n", s);
					if (!exec_commands(shell, cmds) && execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)
					{
						perror("cmd");
						printf("ERR %d\n", errno);
						// exit(get_error_num(errno));
						exit(0);
					}
				}
				exit(EXIT_SUCCESS);
			}
			else if (pid < 0)
			{
				perror("Error");
				exit(EXIT_FAILURE);
			}*/
				if (cmds->append != 0 || (cmds->prev && cmds->prev->append))
				{
					ior[0] = 0;
					ior[1] = fds[1];
					exec_io_redi(cmds, ior[0], ior[1], shell);
					if ((!exec_commands(shell, cmds) && (execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)))
					{
						perror("cmd1");
						//exit(EXIT_FAILURE);
					}
					close(ior[1]);
					ior[1] = 1;
				}
				else if (cmds->args)
				{
					if ((!exec_commands(shell, cmds) && (execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)))
					{
						if ()
						printf("Error: %d | %s\n", errno, strerror(errno));
						perror(cmds->cmd);
						// ft_putstr(strerror(errno));
						exit(EXIT_FAILURE);
					}
				}
				exit(0);
			}
			/* else if (pid < 0)
			{
				perror("Error");
				exit(EXIT_FAILURE);
			} */
			if (cmds->end)
				break;
			else
				cmds = cmds->next;
			j += 2;
			save_restor_fd(0,1);
		}
		for (i = 0; i < 2 * num_pipe; i++)
			close(fds[i]);
		i = -1;
		if (!num_pipe)
			waitpid(pid, &status, 0);
		else
		{
			while (++i < 2 * num_pipe)
			{
				wait(&status);
			}
		}
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

/* int		get_num_sp(t_cmds *cmds)
{
	int	i;

	i = 0;
	while (cmds->s)
	{
		i++;
		cmds = cmds->next;
	}
	return (i);
} */

int		run_commands(t_shell *shell)
{
	t_cmds	*cmds;

	shell = parse_commands(shell);
	cmds = shell->cmds;
	while (cmds)
	{
		//save_restor_fd(1,0);
		cmds = excute_command_by_order(shell, cmds, get_num_pipes(cmds), 1);
		//save_restor_fd(0,1);
		cmds = cmds->next;
	}
	return (1);
}
