
#include "../../includes/shell.h"


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

int     is_quote(char c, int type)
{
    if (type == 1 && c == '\'')
        return (1);
    if (type == 2 && c == '\"')
        return (2);
    if (type == 0 && (c == '\'' || c == '\"'))
        return (c == '\'' ? 1 : 2);
    return (0);
}

void    echo_print(char **str, int pos)
{
    ft_putstr_fd(str[pos], 1);
	if (str[pos + 1])
		ft_putchar_fd(' ', 1);
}

char	*parse_variable(char *arg, char **env, int ret)
{
    int i;
    int pos;
    char *path;

    i = 0;
    pos = 0;
    path = NULL;
    while(arg[i] && !is_quote(arg[i], 0))
    {
        if (arg[i] == '$' && arg[i + 1])
            pos = i + 1;
        i++;
    }
    if (pos && arg[pos] == '?')
    {
        path = ft_itoa(ret);
        if (ret == 0)
            path = ft_strdup("0");
    }
    else if ((i = ft_getenv(arg + pos, env)) >= 0)
        path = env[i] + ft_strlen(arg + pos) + 1;
    arg[pos - 1] = '\0';
    if (path)
        return (ft_strjoin(arg, path));
    else
        return (arg);
}

int     get_special_char(char c)
{
    if (c == 'n')
        return (10);
    else if (c == 'r')
        return (13);
    else if (c == 'v')
        return (11);
    else if (c == 't')
        return (9);
    else if (c == 'f')
        return (12);
    else
        return (c);
}



int		check_n_flag(char *str, int *n)
{
	int i;
	int	is_flag;

	i = 0;
	is_flag = 0;
	if (str[0] == '-' && str[1] == 'n' && (str[2] == 'n' || !str[2]))
    {
        *n = 2;
		is_flag = 1;
		while (str[*n] == 'n')
        	*n = *n + 1;
		if (str[*n])
			is_flag = 0;
    }
	return (is_flag);
}

int			echo_builtin(t_cmds *cmd, t_shell *shell)
{
    int     i;
    int     n;

    i = 0;
    n = 0;
    if (!cmd->args[1])
    {
        ft_putchar_fd('\n', 1);
        return (0);
    }
    while (cmd->args[++i])
    {
		if (!n)
			while (check_n_flag(cmd->args[i], &n))
				i++;
		echo_print(cmd->args, i);
		if (!cmd->args[i + 1] && !n)
			ft_putchar_fd('\n', 1);
    }
    return (0);
}
