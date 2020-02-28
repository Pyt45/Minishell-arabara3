#include <stdio.h>
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

int     main(int argc, char **argv, char **envp)
{
    int     status;
    t_shell shell;
    status = 1;
    //if (argc)
   // {
        //while (status)
        //{
            shell.env = ft_arrdup(envp);
            status = env_command(argv, shell.env);
        //}
   // }
    return (0);
}