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
	int		flag_mode;

	flag = O_WRONLY | O_CREAT;
	flag_mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;
	if (append)
		flag = flag | O_APPEND;
	else
		flag = flag | O_TRUNC;
	
	write_to_file("ARG ", cmd->next->args[0], 1);
	if ((fd = open(cmd->next->args[0], flag, flag_mode)) < 0)
	{
		print_error(cmd->next->args[0], errno, 0);
		exit(1);
	}
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
	else
	{
		if ((fd = open(args, O_RDONLY)) < 0)
		{
			print_error(args, errno, 0);
			exit(1);
		}
		ifd = fd;
	}
	dup2(ifd, 0);
	return (ifd);
}

void		do_redirect(t_cmds *cmd, int fd[2])
{
	if (cmd->append == -2) // << not working now
		fd[0] = open_input(cmd->args[1], 1, fd[0]);
	else if (cmd->append == 2) // >> working
	{
		fd[1] = open_output(cmd, 1, fd[1]);
		//printf("%s %s\n", cmd->args[1], cmd->next->args[0]);
	}
	if (cmd->append == 1) // > working || need to fix echo a > txt b || cat > file
		fd[1] = open_output(cmd, 0, fd[1]);
	else if (cmd->append == -1) // < need fix
	{
		fd[0] = open_input(cmd->next->args[0], 0, fd[0]);
	}
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
	t_cmds *tmp;

	i = 0;
	tmp = cmds;
	while (tmp->p)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int		get_num_rd(t_cmds *cmds)
{
	int	i;
	t_cmds *tmp;

	i = 0;
	tmp = cmds;
	while (tmp->r)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int		is_builtin(char *cmd){
	if (cmd && ft_strcmp(cmd, "env") && ft_strcmp(cmd, "cd") &&
		ft_strcmp(cmd, "pwd") && ft_strcmp(cmd, "exit") &&
		ft_strcmp(cmd, "export") && ft_strcmp(cmd, "unset") &&
		ft_strcmp(cmd, "echo"))
       return (0);
	return (1);
}

char	**clear_args(char **args, t_shell *shell){
	int i;

	i = 0;
	while (args[i])
	{
		// args[i] = clear_quotes(args[i]);
		if (ft_strchr(args[i] ,'\\'))
			args[i] = parse_special_chars(args[i]);
		// args[i] = replace_string(args[i], shell);
		i++;
	}
	return (args);
}

int     exec_commands(t_shell *shell, t_cmds *cmds)
{
	int	ret;

	ret = 1;
	//printf("cmds = %s\nargs = %s\n", cmds->cmd, cmds->args[0]);
	if (!cmds->cmd || !cmds->cmd[0])
		return (0);
	cmds->cmd = clear_quotes(cmds->cmd);
	// cmds->args = clear_args(cmds->args, shell);
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
	else if(!cmds->prev || cmds->prev->append == 0)
		execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env);
	else
		ret = 0;
    return (ret);
}

void	close_pipes(int *fds, int num_pipe)
{
	int	i;

	i = 0;
	while (i < 2 * num_pipe)
		close(fds[i++]);
}

static pid_t	run_child(t_shell *shell, t_cmds *cmds, int j)
{
	pid_t	pid;
	int		in;
	in = 0;

	pid = fork();
	if (pid == 0)
	{
		(shell->num_pipe) ? shell->fds = create_fds(cmds, j, shell->fds) : 0;
		close_pipes(shell->fds, shell->num_pipe);
		if (cmds->append != 0 || (cmds->prev && cmds->prev->append))
		{
			exec_io_redi(cmds, in, shell->fds[1], shell);
			if (cmds->args && exec_commands(shell, cmds))
			{
				print_error(cmds->cmd, errno, 1);
				exit(1);
			}
			close(shell->fds[1]);
			in = shell->fds[0];
		}
		else if (cmds->args && exec_commands(shell, cmds))
		{
				print_error(cmds->cmd, errno, 1);
				exit(1);
		}
		exit(0);
	}
	else if (pid < 0)
	{
		perror("Error");
		exit(1);
	}
	return (pid);
}

static int wait_child(t_shell *shell, pid_t pid, int st)
{
	int		i;

	i = -1;
	if (!shell->num_pipe)
		waitpid(pid, &st, 0);
	else
	{
		while (++i < 2 * shell->num_pipe)
			wait(&st);
	}
	return (st);
}

t_cmds     *excute_command_by_order(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;
	int 	status = 0;
	int		*ior;
	int		j = 0;
	
	//(num_pipe) ? fds = pipe_fds(num_pipe, fds) : 0;
	//(num_sp) ? fds = pipe_ior(num_sp, fds) : 0;
	//pipe_ior(shell->num_sp, ior);
	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		shell->fds = pipe_fds(shell->num_pipe, shell->fds);
		// (num_sp) ? fds = pipe_ior(num_sp, fds) : 0;
		j = 0;
		while (cmds)
		{
			//save_restor_fd(1,0);
			// pid = fork();
			// if (pid == 0)
			// {
			// 	(num_pipe) ? fds = create_fds(cmds, j, fds) : 0;
			// 	close_pipes(fds, num_pipe);
			// 	if (cmds->append != 0 || (cmds->prev && cmds->prev->append))
			// 	{
			// 		//ior[0] = 0;
			// 		//ior[1] = fds[1];
			// 		//exec_io_redi(cmds, ior[0], ior[1], shell);
			// 		if (cmds->args && !exec_commands(shell, cmds))
			// 		{
			// 			print_error(cmds->cmd, errno, 1);
			// 			// exit(1);
			// 		}
			// 		//close(ior[1]);
			// 		//ior[1] = 1;
			// 	}
			// 	else if (cmds->args && exec_commands(shell, cmds))
			// 	{
			// 		print_error(cmds->cmd, errno, 1);
			// 		exit(1);
			// 	}
			// 	exit(0);
			// }
			pid = run_child(shell, cmds, j);
			if (cmds->end)
				break;
			else
				cmds = cmds->next;
			j += 2;
			//save_restor_fd(0,1);
		}
		close_pipes(shell->fds, shell->num_pipe);
		status = wait_child(shell, pid, status);
		cmds->ret = status;
		free(shell->fds);
	}
	else
		cmds->ret = exec_commands(shell, cmds);
    return (cmds);
}

int		run_commands(t_shell *shell)
{
	t_cmds	*cmds;
	shell->num_sp = 0;
	shell->num_pipe = 0;
	shell->parse_err = 0;
	shell = parse_commands(shell);
	if (shell->parse_err == -1)
		print_error("syntax error", 0, 0);
	else {
		cmds = shell->cmds;
		while (cmds)
		{
			//save_restor_fd(1,0);
			shell->num_pipe = get_num_pipes(cmds);
			shell->num_sp = get_num_rd(cmds);
			cmds = excute_command_by_order(shell, cmds);
			//save_restor_fd(0,1);
			shell->ret = cmds->ret;
			cmds = cmds->next;
		}
	}
	return (1);
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
// 	// printf("%d\n", num_pipe);
// 	(num_sp) ? fds = pipe_ior(num_sp, fds) : 0;
// 	if ((cmds->next && !cmds->end) || exec_commands(shell, cmds))
// 	{
// 		fds = pipe_fds(num_pipe, fds);
// 		j = 0;
// 		while (cmds)
// 		{
// 			//save_restor_fd(1,0);
// 			pid = fork();
// 			if (pid == 0)
// 			{
// 				//signal(SIGINT, sig_handle_ctrl_c);
// 				//signal(SIGQUIT, SIG_DFL);
// 				(num_pipe) ? fds = create_fds(cmds, j, fds) : 0;
// 				for (i = 0; i < 2 * num_pipe; i++)
// 					close(fds[i]);
// 				if (cmds->append != 0 || (cmds->prev && cmds->prev->append))
// 				{
// 					ior[0] = 0;
// 					ior[1] = fds[1];
// 					exec_io_redi(cmds, ior[0], ior[1], shell);
// 					if ((exec_commands(shell, cmds) && (execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)))
// 					{
// 						print_error(cmds->cmd, errno, 1);
// 						// exit(1);
// 					}
// 					close(ior[1]);
// 					ior[1] = 1;
// 				}
// 				else if (cmds->args)
// 				{
// 					if ((exec_commands(shell, cmds) && (execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env) < 0)))
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
// 			//save_restor_fd(0,1);
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