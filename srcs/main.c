#include "../includes/shell.h"
#include <signal.h>
// #include <readline/readline.h>

void	free_shell(t_shell *shell)
{
	int i;
	t_cmds *tmp;
	// free all the data and re-init
	free(shell->line);
	while(shell->cmds)
	{
		i = 0;
		while (shell->cmds->args[i])
		{
			ft_del(*(shell->cmds->args));
			i++;
		}
		ft_del(shell->cmds->args);
		ft_del(shell->cmds->cmd);
		tmp = shell->cmds->next;
		ft_del(shell->cmds);
		shell->cmds = tmp;
	}
	shell->cmds = NULL;
}

// int		command_line(t_shell *shell)
// {
// 	int		status;

// 	status = 1;
// 	init_config(&shell->config);
// 	ft_putstr_fd("\033[1;32mminishell~>\033[0m", 1);
// 	init_config_data(&shell->config);
// 	shell->ret = 0;
// 	while (status)
// 	{
// 		read(0, &shell->config.buff, sizeof(&shell->config.buff));
// 		handle_keys(&shell->config);
// 		if (ft_isprint(shell->config.buff))
//             print_char(&shell->config);
// 		if (shell->config.buff == ENTER_BTN)
//     	{
// 			shell->line = shell->config.str;
// 			newline_config(&shell->config);
// 			shell->config.term.c_lflag |= (ICANON | ECHO);
// 			tcsetattr(0, 0, &shell->config.term);
// 			if (ft_strlen(shell->line))
// 				status = run_commands(shell);
// 			if (status)
//         		re_init_shell(&shell->config);
//     	}
// 		//free_shell(shell);
// 		shell->config.buff = 0;
// 	}
// 	return (status);
// }

char	*read_line(t_shell *shell)
{
	init_prompt(&shell->config);
	while (read(0, &shell->config.buff, sizeof(&shell->config.buff)))
	{
		handle_keys(&shell->config);
		write_to_file("read ", "ok", 1);
		if (ft_isprint(shell->config.buff))
            print_char(&shell->config);
		if (shell->config.buff == ENTER_BTN)
    	{
			write_to_file("enter ", "ok", 1);
			shell->line = shell->config.str;
			newline_config(&shell->config);
			// if (ft_strlen(shell->line))
			break;
    	}
		shell->config.buff = 0;
	}
	return (shell->line);
}


int		command_line(t_shell *shell)
{
	int		status;

	status = 1;
	// init_config(&shell->config);
	// ft_putstr_fd("\033[1;32mminishell~>\033[0m", 2);
	// init_config_data(&shell->config);
	// shell->ret = 0;
	// while (status)
	// {
	// 	read(0, &shell->config.buff, sizeof(&shell->config.buff));
	// 	handle_keys(&shell->config);
	// 	if (ft_isprint(shell->config.buff))
    //         print_char(&shell->config);
	// 	if (shell->config.buff == ENTER_BTN)
    // 	{
	// 		shell->line = shell->config.str;
	// 		newline_config(&shell->config);
	// 		shell->config.term.c_lflag |= (ICANON | ECHO);
	// 		tcsetattr(0, 0, &shell->config.term);
	// 		if (ft_strlen(shell->line))
	// 			status = run_commands(shell);
	// 		if (status)
    //     		re_init_shell(&shell->config);
    // 	}
	// 	//free_shell(shell);
	// 	shell->config.buff = 0;
	// }
	shell->ret = 0;
	while ((shell->line = read_line(shell)))
		status = run_commands(shell);
	return (status);
}


void	sig_handle_ctrl_c(int sig)
{
	sig = 0;
	// signal(SIGINT, sig_handle_ctrl_c);
}

void	erase_file_debug()
{
	FILE *f;
	int fd;

	f = fopen("debug.txt", "w");
	fd = fileno(f);
	ft_putstr_fd("", fd);
	fclose(f);
}


int     main(int argc, char **argv, char **envp)
{
	t_shell shell;

	erase_file_debug();
	signal(SIGINT, sig_handle_ctrl_c);
	// signal(SIGQUIT, SIG_IGN);
	if (argc && argv)
	{
		shell.env = ft_arrdup(envp);
		//signal(SIGINT, &sig_handle_ctrl_c);		
		command_line(&shell);
	}
    return (0);
}