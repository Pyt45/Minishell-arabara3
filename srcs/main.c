

#include "../includes/shell.h"


int     env_command(char **argv, char **env)
{
    if (argv[1] != NULL)
        ft_putstr_fd("Error Env has no arg\n", 2);
	else
   		ft_print_env_arr(env);
    return (1);
}

t_cmds    *get_test()
{
    t_cmds *cmds;
    t_cmds *head;
    cmds = malloc(sizeof(t_cmds));
    head = cmds;
    cmds->cmd = "env";
    cmds->args = NULL;
    cmds->next = malloc(sizeof(t_cmds));
    cmds = cmds->next;
    cmds->cmd = "grep";
    cmds->args = malloc(sizeof(char *));
	cmds->args[0] = "PATH";
    /* cmds->next = malloc(sizeof(t_cmds));
    cmds = cmds->next;
    cmds->cmd = "cat";
    *cmds->args[2] = "-e"; */
    cmds->next = NULL;
    return head;   
}

int		command_line(t_shell *shell)
{
	int		r;

	r = 1;
	ft_putstr_fd("minishell>", 1);
	while (r == 1)
	{
		r = get_next_line(0, &shell->line);
		excute_command_by_order(shell);
		free(shell->line);
	}
	return (r);
}

int		sig_handle_ctrl_c(int signal)
{
	if (signal)
		exit(EXIT_FAILURE);
	return (0);
}

int     main(int argc, char **argv, char **envp)
{
	t_shell shell;
    if (argc && argv)
	{
		shell.env = ft_arrdup(envp);
		//signal(SIGINT, sig_handle_ctrl_c);
		while (command_line(&shell))
			;
	}

    return (0);
}