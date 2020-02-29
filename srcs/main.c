
#include "../includes/shell.h"

int     ft_strlen(char *str)
{
    int     i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char    *ft_strcpy(char *s1, char *s2)
{
    int     i;

    i = 0;
    while (s2[i] != '\0')
    {
        s1[i] = s2[i];
        i++;
    }
    s1[i] = '\0';
    return (s1);
}

void    ft_putchar_fd(char ch, int fd)
{
    write(fd, &ch, 1);
}

void    ft_putstr_fd(char *str, int fd)
{
    while (*str)
    {
        ft_putchar_fd(*str, fd);
        str++;
    }
}

int     ft_arr_len(char **arr)
{
    int     i;

    i = 0;
    if (arr)
    {
        while (arr[i] != NULL)
            i++;
    }
    return (i);
}

char    **ft_arrdup(char **arr)
{
    int     i;
    int     len;
	size_t	str_len;
    char    **new_arr;

    len = ft_arr_len(arr);
    if (!(new_arr = (char **)malloc(sizeof(char *) * (len + 1))))
        return (NULL);
    i = 0;
    while (i < len)
    {
        str_len = (int)ft_strlen(arr[i]);
        if (!(new_arr[i] = (char *)malloc(sizeof(char) * (str_len + 1))))
            return (NULL);
        ft_strcpy(new_arr[i], arr[i]);
        i++;
    }
    new_arr[i] = NULL;
    return (new_arr);
}

void    ft_print_env_arr(char **arr)
{
    int     i;

    i = 0;
    if (arr)
    {
        while (arr[i] != NULL)
		{
            ft_putstr_fd(arr[i], 1);
			ft_putstr_fd("\n", 1);
        	i++;
		}
    }
}

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
    *cmds->args[0] = NULL;
    cmds->next = malloc(sizeof(t_cmds));
    cmds = cmds->next;
    cmds->cmd = "grep";
    *cmds->args[1] = "PATH";
    cmds->next = malloc(sizeof(t_cmds));
    cmds = cmds->next;
    cmds->cmd = "cat";
    *cmds->args[2] = "-e";
    cmds->next = NULL;
    return head;   
}

int     main(int argc, char **argv, char **envp)
{
    int     pid;
    int     status;
    int     fds[2];
    t_shell shell;
    t_cmds  *cmds;

    shell.env = ft_arrdup(envp);
    cmds = get_test();
    if (cmds->cmd && (*cmds->args[0] == NULL) && !cmds->next)
        return (env_command(argv, shell.env));
    //else
    //{
    pipe(fds);
    if ((pid == fork()) == 0)
    {
        dup2(fds[0], 0);
        close(fds[0]);
        close(fds[1]);

        if (execve(get_bin_path(argv[0], shell.env), argv, shell.env) < 0)
            exit(0);
    }
    else
    {
        close(fds[0]);
        dup2(fds[1], 1);
        close(fds[1]);
    }
   // }
    status = waitpid(pid, &status, 0);
    return (0);
}