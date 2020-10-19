#include "../includes/shell.h"
#include <signal.h>

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

int		command_line(t_shell *shell)
{
	int		status;

	status = 1;
	init_config(&shell->config);
	ft_putstr_fd("\033[1;32mminishell~>\033[0m", 1);
	init_config_data(&shell->config);
	while (status)
	{
		read(0, &shell->config.buff, sizeof(&shell->config.buff));
		handle_keys(&shell->config);
		if (ft_isprint(shell->config.buff))
            print_char(&shell->config);
		if (shell->config.buff == ENTER_BTN)
    	{
			shell->line = shell->config.str;
			// write_to_file("COMMAND: ", shell->line, 1);
			newline_config(&shell->config);
			if (ft_strlen(shell->line))
				status = run_commands(shell);
			if (status)
			{
			// 	write_to_file("BEFORE INIT => ", "", 0);
			// 	write_to_file("OX: ", ft_itoa(shell->config.o_x), 0);
			// 	write_to_file(" | NX: ", ft_itoa(shell->config.x), 0);
			// 	write_to_file(" | OY: ", ft_itoa(shell->config.o_y), 0);
			// 	write_to_file(" | NY: ", ft_itoa(shell->config.y), 1);

        		re_init_shell(&shell->config);
				// write_to_file("AFTER INIT => ", "", 0);
				// write_to_file("OX: ", ft_itoa(shell->config.o_x), 0);
				// write_to_file(" | NX: ", ft_itoa(shell->config.x), 0);
				// write_to_file(" | OY: ", ft_itoa(shell->config.o_y), 0);
				// write_to_file(" | NY: ", ft_itoa(shell->config.y), 1);
			}
    	}
		//free_shell(shell);
		shell->config.buff = 0;
	}
	return (status);
}

void	sig_handle_ctrl_c()
{
	signal(SIGINT, sig_handle_ctrl_c);
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
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (argc && argv)
	{
		shell.env = ft_arrdup(envp);
		//signal(SIGINT, &sig_handle_ctrl_c);		
		command_line(&shell);
	}
    return (0);
}