#include "../includes/shell.h"

int		exit_builtin(t_shell *shell, t_cmds *cmds)
{
    double  tstatus;
    int     status;
	int     i;

	tstatus = 0;
	i = -1;
	if (cmds->args[1])
    {
        while (cmds->args[1][i++])
            if (ft_isalpha((int)(cmds->args[1][i])))
            {
                tstatus = 1;
                break;
            }
        status = ft_atoi(cmds->args[1]);
    }
    end_terminal(&shell->config);
	free_shell(shell);
	free_config(&shell->config);
    ft_free_arr(shell->env);
    ft_putstr_fd("exit\n", 1);
	if (tstatus && !status)
		print_error("exit", 33, 0);
    exit(status);
    return (0);
}

void	reinit_cursor(t_config *config, int new_x, int new_y)
{
	config->o_x = new_x;
	config->o_y = new_y;
	config->x = config->o_x;
	config->y = config->o_y - 1;
	config->c = 0;
	config->len = 0;
	ft_bzero(config->str, 512 * sizeof(char));
	move_cursor(config, 3);
}

void	validate_cursor(t_config *config)
{
	int 	new_y;
	int		new_x;
	int		i;
	char	buff[20];

	i = 0;
	ft_bzero(buff, sizeof(char) * 20);
	ft_putstr_fd("\e[6n", 2);
	while (buff[0] != '\e')
	{
		read(2, buff, sizeof(buff));
		if (buff[0] != '\e')
			ft_putstr_fd("\e[6n", 2);
	}
	while (!ft_isdigit(buff[i]))
		i++;
	new_y = ft_atoi(buff + i) - 1;
	while (ft_isdigit(buff[i]))
			i++;
	new_x = ft_atoi(buff + i + 1);
	if (new_y > config->y || (new_y == config->y && new_x ==
		config->o_x && ( config->len < config->width - 1 ||
		config->len > config->width)) || (config->o_x ==
		new_x && config->len == config->width))
		reinit_cursor(config, new_x, new_y);
}

char	*clear_str(char *str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i])
	{
		
		if (str[i] == ' ')
		{
			j = i;
			while (str[j])
			{
				str[j] = str[j + 1];
				j++;
			}
			i--;
		}
		else
		 	break;
		i++;
	}
	return (str);
}

char	*read_line(t_shell *shell)
{
	init_prompt(&shell->config, shell->ret);
	while ((*shell->config.str && shell->config.tmp) || shell->config.buff || read(0, &shell->config.buff, sizeof(&shell->config.buff)))
	{
		validate_cursor(&shell->config);
		handle_keys(&shell->config);
		if (ft_isprint(shell->config.buff))
            print_char(&shell->config);
		if (shell->config.buff == ENTER_BTN)
    	{
			shell->config.str = clear_str(shell->config.str);
			shell->line = shell->config.str;
			newline_config(&shell->config, 0);
			end_terminal(&shell->config);
			shell->config.buff = 0;
			break;
    	}
		shell->config.buff = 0;
	}
	return (shell->line);
}

void		command_line(t_shell *shell)
{
	shell->ret = 0;
	while ((shell->line = read_line(shell)))
	{
		if (ft_strlen(shell->line))
			run_commands(shell);
		free_shell(shell);
	}
}

void	sig_handle_ctrl_c(int sig)
{
	sig = 0;
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("\033[1;32mminishell~>\033[0m", 1);
}

int     main(int argc, char **argv, char **envp)
{
	t_shell shell;

	erase_file_debug();
	signal(SIGINT, sig_handle_ctrl_c);
	if (argc && argv)
	{
		init_shell(&shell);
		shell.env = ft_arrdup(envp);
		command_line(&shell);
	}
    return (0);
}