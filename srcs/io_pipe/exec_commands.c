#include "../../includes/shell.h"


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
// ==================== io_redirection.c file=========================Start
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
	return (ifd);
}

int		open_output(t_cmds *cmd, int append)
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
	if ((fd = open(cmd->next->args[0], flag, flag_mode)) < 0)
	{
		print_error(cmd->next->args[0], errno, 0);
		exit(1);
	}
	return (fd);
}

// ==================== io_redirection.c file=========================End

//================================exec_redirection.c file==============Start
void		redirect_forward(t_cmds *tmp, t_cmds *cmd, int *fd)
{
	int i;

	if (ft_arr_len(tmp->next->args) > 1 && !tmp->next->start)
	{
		i = 1;
		while (tmp->next->args[i])
		{
			cmd->args = ft_get_arr(tmp->next->args[i], cmd->args);
			i++;
		}
	}
	if (fd[1])
		close(fd[1]);
	fd[1] = open_output(tmp, tmp->append - 1);
}

void		redirect_backward(t_cmds *tmp, int *fd)
{
	int		i;
	char	*file;

	i = -1;
	if (fd[0])
		close(fd[0]);
	while(tmp->next->args[++i])
		file = tmp->next->args[i];
	fd[0] = open_input(file, 0, fd[0]);
}

void		do_redirect(t_cmds *cmd, int *fd)
{
	t_cmds	*tmp;

	tmp = cmd;
	while (tmp->append)
	{
		if (tmp->append > 0)
			redirect_forward(tmp, cmd, fd);
		else
			redirect_backward(tmp, fd);
		tmp = tmp->next;
	}
	if (fd[1])
		dup2(fd[1], 1);
	if (fd[0])
		dup2(fd[0], 0);

	// if (cmd->append == -2) // << not working now
	// 	fd[0] = open_input(cmd->args[1], 1, fd[0]);
	// else if (cmd->append > 0) // >> working
	// {
	// 	tmp = cmd;
	// 	while (tmp->append > 0)
	// 	{
	// 		if (fd[1])
	// 			close(fd[1]);
	// 		fd[1] = open_output(tmp, tmp->append - 1);
	// 		tmp = tmp->next;
	// 	}
	// }
	// else if (cmd->append == -1)
	// {
	// 	fd[0] = open_input(cmd->next->args[0], 0, fd[0]);
	// }
}




// void		do_redirect(t_cmds *cmd, int *fd)
// {
// 	t_cmds	*tmp;

// 	// ls -l | wc > out // working
// 	// cat < out > out1 // need to be fixed

	

// 	if (cmd->append == -2) // << not working now
// 		fd[0] = open_input(cmd->args[1], 1, fd[0]);
// 	else if (cmd->append > 0) // >> working
// 	{
// 		tmp = cmd;
// 		while (tmp->append > 0)
// 		{
// 			if (fd[1])
// 				close(fd[1]);
// 			fd[1] = open_output(tmp, tmp->append - 1);
// 			tmp = tmp->next;
// 		}
// 		dup2(fd[1], 1);
// 	}
// 	else if (cmd->append == -1) // < need fix
// 		fd[0] = open_input(cmd->next->args[0], 0, fd[0]);
// }

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
//================================exec_redirection.c file==============End



//================================exec_help_func.c file==============Start
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

void	close_pipes(int *fds, int num_pipe)
{
	int	i;

	i = 0;
	while (i < 2 * num_pipe)
		close(fds[i++]);
}

int		is_builtin(char *cmd){
	if (cmd && ft_strcmp(cmd, "env") && ft_strcmp(cmd, "cd") &&
		ft_strcmp(cmd, "pwd") && ft_strcmp(cmd, "exit") &&
		ft_strcmp(cmd, "export") && ft_strcmp(cmd, "unset") &&
		ft_strcmp(cmd, "echo"))
       return (0);
	return (1);
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

int		get_status_number(int status)
{
	if (status > 200)
		return (status - 129);
	else if (status == 2)
		return (130);
	else if (status == 3)
		return (131);
	return (status);
}


//================================exec_help_func.c file==============End

//================================run_child.c file==============Start 

int     exec_commands(t_shell *shell, t_cmds *cmds)
{
	int	ret;

	ret = 1;
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
	{
		if (cmds->cmd[0] == '/' || (cmds->cmd[0] == '.' && cmds->cmd[1] == '/'))
			execve(cmds->cmd, cmds->args, shell->env);
		else
			execve(get_bin_path(cmds->cmd, shell->env), cmds->args, shell->env);
	}
	else
		ret = 0;
    return (ret);
}

static void	child_help(t_shell *shell, t_cmds *cmds, int *ior)
{
	ior[0] = 0;
	ior[1] = shell->fds[1];
	exec_io_redi(cmds, ior[0], ior[1], shell);
	if (cmds->args && exec_commands(shell, cmds))
	{
		print_error(cmds->cmd, errno, 1);
		exit(1);
	}
	if (ior[1] != shell->fds[1])
		close(shell->fds[1]);
	ior[1] = 1;
}

static pid_t	run_child(t_shell *shell, t_cmds *cmds, int j)
{
	pid_t	pid;
	int		in;
	int		ior[2];
	ior[1] = 0;

	pid = fork();
	if (pid == 0)
	{
		
		(shell->num_pipe) ? shell->fds = create_fds(cmds, j, shell->fds) : 0;
		close_pipes(shell->fds, shell->num_pipe);
		if (cmds->append != 0 || (cmds->prev && cmds->prev->append))
		{
			child_help(shell, cmds, ior);
			// ior[0] = 0;
			// ior[1] = shell->fds[1];
			// exec_io_redi(cmds, ior[0], ior[1], shell);
			// if (cmds->args && exec_commands(shell, cmds))
			// {
			// 	print_error(cmds->cmd, errno, 1);
			// 	exit(1);
			// }
			// if (ior[1] != shell->fds[1])
			// 	close(ior[1]);
			// ior[1] = 1;
		}
		else if (cmds->args && exec_commands(shell, cmds) && !is_builtin(cmds->cmd))
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

//================================run_child.c file==============End

static void	excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid)
{
	int		status;

	status = 0;
	close_pipes(shell->fds, shell->num_pipe);
	status = wait_child(shell, pid, status);
	cmds->ret = get_status_number(status);
	free(shell->fds);
}

static t_cmds *excute_codition(t_cmds *cmds)
{
	while(cmds->append)
			cmds = cmds->next;
	return (cmds);
}

t_cmds     *excute_command_by_order(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;
	int 	status = 0;
	int		*ior;
	int		j = 0;
	
	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		//excute_cmd_help(shell, cmds, j);
		shell->fds = pipe_fds(shell->num_pipe, shell->fds);
		j = 0;
		while (cmds)
		{
			if (cmds->end && cmds->prev && cmds->prev->append)
				break;
			pid = run_child(shell, cmds, j);
			cmds = excute_codition(cmds);
			if (cmds->end)
				break;
			else
				cmds = cmds->next;
			j += 2;
		}
		excute_cmd_help(shell, cmds, pid);
	}
	else if (cmds->cmd)
		cmds->ret = exec_commands(shell, cmds);
    return (cmds);
}

int		run_commands(t_shell *shell)
{
	t_cmds	*cmds;

	shell->num_pipe = 0;
	shell->parse_err = 0;
	shell = parse_commands(shell);
	if (shell->parse_err == -1)
		print_error("syntax error", 0, 0);
	else {
		cmds = shell->cmds;
		while (cmds)
		{
			shell->num_pipe = get_num_pipes(cmds);
			cmds = excute_command_by_order(shell, cmds);
			shell->ret = cmds->ret;
			cmds = cmds->next;
		}
	}
	return (1);
}