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
	int		r;
	int		status;
	int		column_count;
	int		line_count;
	r = 1;
	status = 1;
	char *buffer = malloc(2048);
	tgetent(buffer, getenv("TERM")); //TERM = xterm-256color
	char *keyup = tgetstr("ku", &buffer);
	// int ret = setupterm(NULL, STDOUT_FILENO, NULL);
	while (status)
	{
		printf("%s\n", keyup);
		// column_count = tigetnum("cols");
		// line_count = tigetnum("lines");
		// printf("%d %d\n", column_count, line_count);
		ft_putstr_fd("\033[1;32mminishell~>\033[0m", 1);
		// column_count = tigetnum("cols");
		// line_count = tigetnum("lines");
		// printf("%d %d\n", column_count, line_count);
		// int key = getch();
		// printf("%d\n", key);

		// char* color_cap = tigetstr("setaf");
		// tputs(tparm(color_cap, COLOR_RED), 1, putchar);
		// printf("Cool ! Maintenant j'ecris en rouge !\n");
		
		r = get_next_line(0, &shell->line);
		if (ft_strlen(shell->line))
			status = run_commands(shell);
		//free_shell(shell);
	}
	return (status);
}

void	sig_handle_ctrl_c(int signal)
{
	if (signal)
		exit(EXIT_FAILURE);
}

int     main(int argc, char **argv, char **envp)
{
	t_shell shell;

	signal(SIGINT, SIG_IGN);
	if (argc && argv)
	{
		shell.env = ft_arrdup(envp);
		//signal(SIGINT, sig_handle_ctrl_c);
		while (command_line(&shell))
			;
	}
    return (0);
}