
#include "../includes/shell.h"

char    *ft_strcat(char *dest, char *src)
{
    int     i;
    int     len;
    i = 0;
    len = ft_strlen(dest);
    while (dest[i] != '\0')
    {
        dest[i + len] = src[i];
        i++;
    }
    dest[i + len] = '\0';
    return (dest);
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

static void	ft_putnstr(char *str, int n)
{
	int		i;

	i = -1;
	if (n < 0)
	{
		while (str[++i] && i < (int)ft_strlen(str) + n)
			ft_putchar_fd(str[i], 1);
	}
	else
	{
		while (str[++i] && i < n)
			ft_putchar_fd(str[i], 1);
	}
}

# define IS_QUOTE(x) (x == '"' || x == '\'')

static void echo_print(char **str, int pos)
{
    int     s_with;
    int     e_with;
    int     len;

    s_with = IS_QUOTE(str[pos][0]);
	len = (int)ft_strlen(str[pos]);
	e_with = IS_QUOTE(str[pos][len - 1]);
	if (e_with && s_with)
		ft_putnstr(str[pos] + 1, -1);
	else if (e_with)
		ft_putnstr(str[pos], -1);
	else if (s_with)
		ft_putstr_fd(str[pos] + 1, 1);
	else
		ft_putstr_fd(str[pos], 1);
	if (str[pos + 1])
		ft_putchar_fd(' ', 1);
}

char    *parse_variable(char *arg, char **env)
{
    int i;
    int pos;
    char *path;

    i = 0;
    while(arg[i])
    {
        if (arg[i] == '$')
            pos = i + 1;
        i++;
    }
    if ((i = ft_getenv(arg + pos, env)) >= 0)
		path = env[i] + ft_strlen(arg + pos) + 1;
    //printf("%c\n", arg[pos - 1]);
    arg[pos - 1] = '\0';
    return (ft_strjoin(arg, path));
}

int     echo_command(t_cmds *cmd, char **env)
{
    int     i;
    int     n_flag;

    n_flag = 0;
    if (!cmd->args[1])
    {
        write(1, "\n", 1);
        return (1);
    }
    else if (cmd->args[1][0] == '-' && cmd->args[1][1] == 'n' && cmd->args[1][2] == '\0')
        n_flag = 1;
    i = 0;
    if (n_flag)
        ++i;
    while (cmd->args[++i])
    {
        if (ft_strchr(cmd->args[i], '$'))
            cmd->args[i] = parse_variable(cmd->args[i], env);
        echo_print(cmd->args, i);
        if (!cmd->args[i + 1] && !n_flag)
            ft_putchar_fd('\n', 1);
    }
    return (1);
}

// ---------------------- TEST ----------------

int main(int argc, char **argv, char **envp)
{
    t_cmds *cmd;
    cmd = malloc(sizeof(t_cmds));
    cmd->cmd = "echo";
    cmd->args = malloc(sizeof(char *) * 3);
    cmd->args[0] = "echo";
    cmd->args[1] = ft_strdup("oh_$HOME");
    cmd->args[2] = 0;
    echo_command(cmd, envp);
}