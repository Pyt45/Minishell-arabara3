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

int		is_builtin(char *cmd){
	if (cmd && ft_strcmp(cmd, "env") && ft_strcmp(cmd, "cd") &&
		ft_strcmp(cmd, "pwd") && ft_strcmp(cmd, "exit") &&
		ft_strcmp(cmd, "export") && ft_strcmp(cmd, "unset") &&
		ft_strcmp(cmd, "echo"))
       return (0);
	return (1);
}

int     exec_commands(t_shell *shell, t_cmds *cmds)
{
	int	ret;

	ret = 0;
	if (!cmds->cmd)
		return (0);
    if (!ft_strcmp(cmds->cmd, "env"))
        ret = env_builtin(cmds, shell->env);
	else if (!ft_strcmp(cmds->cmd, "cd"))
        ret = cd_builtin(shell, cmds);
    else if (!ft_strcmp(cmds->cmd, "pwd"))
        ret = pwd_builtin();
    else if (!ft_strcmp(cmds->cmd, "exit"))
        ret = exit_builtin(shell, cmds);
    else if (!ft_strcmp(cmds->cmd, "export"))
        ret = export_builtin(shell, cmds);
    else if (!ft_strcmp(cmds->cmd, "unset"))
        ret = unset_builtin(shell, cmds);
    else if (!ft_strcmp(cmds->cmd, "echo"))
       ret = echo_builtin(cmds, shell);
	else
		execve(get_bin_path(clear_quotes(cmds->cmd), shell->env), cmds->args, shell->env);
	// printf("exec ret: %d \n", ret);
    return (0);
}


// t_cmds     *excute_command_by_order(t_shell *shell, t_cmds *cmds, int num_pipe, int num_sp)
// {
// 	pid_t	pid;
// 	int 	status = 0;
// 	char	*line;
// 	char	*content;
// 	int		r = 1;
//     int		i = 0;
// 	int		*ior;
// 	int		*fds;
// 	int		j = 0;
	
// 	//num_pipe = 1;
// 	(num_pipe) ? fds = pipe_fds(num_pipe, fds) : 0;
// 	(num_sp) ? fds = pipe_ior(num_sp, fds) : 0;
// 	if ((cmds->next && !cmds->end) || !exec_commands(shell, cmds))
// 	{
// 		//fds = pipe_fds(num_pipe, fds);
// 		j = 0;
// 		while (cmds)
// 		{
// 			save_restor_fd(1,0);
// 			pid = fork();
// 			if (pid == 0)
// 			{

// 				(num_pipe) ? fds = create_fds(cmds, j, fds) : 0;
// 				for (i = 0; i < 2 * num_pipe; i++)
// 					close(fds[i]);
// 				if (cmds->append != 0 || (cmds->prev && cmds->prev->append))
// 				{
// 					ior[0] = 0;
// 					ior[1] = fds[1];
// 					exec_io_redi(cmds, ior[0], ior[1], shell);
// 					if ((!exec_commands(shell, cmds) && (execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)))
// 					{
// 						print_error(cmds->cmd, errno, 1);
// 						// exit(1);
// 					}
// 					close(ior[1]);
// 					ior[1] = 1;
// 				}
// 				else if (cmds->args)
// 				{
// 					if ((!exec_commands(shell, cmds) && (execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)))
// 					{
// 						print_error(cmds->cmd, errno, 1);
// 						exit(1);
// 					}
// 				}
// 				exit(0);
// 			}
// 			/* else if (pid < 0)
// 			{
// 				perror("Error");
// 				exit(EXIT_FAILURE);
// 			} */
// 			if (cmds->end)
// 				break;
// 			else
// 				cmds = cmds->next;
// 			j += 2;
// 			save_restor_fd(0,1);
// 		}
// 		for (i = 0; i < 2 * num_pipe; i++)
// 			close(fds[i]);
// 		i = -1;
// 		if (!num_pipe)
// 			waitpid(pid, &status, 0);
// 		else
// 		{
// 			while (++i < 2 * num_pipe)
// 			{
// 				wait(&status);
// 			}
// 		}
// 		cmds->ret = status;
// 		free(fds);
// 	}
//     return (cmds);
// }

void	close_pipes(int *fds, int num_pipe){
	int	i;

	i = 0;
	while (i < 2 * num_pipe)
		close(fds[i++]);
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
	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
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
				close_pipes(fds, num_pipe);
				if (cmds->append != 0 || (cmds->prev && cmds->prev->append))
				{
					ior[0] = 0;
					ior[1] = fds[1];
					exec_io_redi(cmds, ior[0], ior[1], shell);
					if (!exec_commands(shell, cmds))
					{
						print_error(cmds->cmd, errno, 1);
						// exit(1);
					}
					close(ior[1]);
					ior[1] = 1;
				}
				else if (cmds->args && !exec_commands(shell, cmds))
				{
					print_error(cmds->cmd, errno, 1);
					exit(1);
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
		close_pipes(fds, num_pipe);
		i = -1;
		if (!num_pipe)
			waitpid(pid, &status, 0);
		else
		{
			while (++i < 2 * num_pipe)
				wait(&status);
		}
		cmds->ret = status;
		free(fds);
	} else {
		cmds->ret = exec_commands(shell, cmds);
	}
    return (cmds);
}

int		run_commands(t_shell *shell)
{
	t_cmds	*cmds;

	shell = parse_commands(shell);
	cmds = shell->cmds;
	// clear_quotes(cmds);
	while (cmds)
	{
		//save_restor_fd(1,0);
		cmds = excute_command_by_order(shell, cmds, get_num_pipes(cmds), 1);
		//save_restor_fd(0,1);
		shell->ret = cmds->ret;
		cmds = cmds->next;
	}
	return (1);
}
