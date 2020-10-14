
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

static int	parse_quotes(char **str)
{
    int     s_with;
    int     e_with;
    int     quote;
    int     len;

    quote = 0;
    quote = IS_S_QUOTE(*str[0]);
    quote = IS_D_QUOTE(*str[0]) ? 2 : quote;
    s_with = IS_QUOTE(*str[0]);
	len = (int)ft_strlen(*str);
    e_with = IS_QUOTE((*str)[len - 1]);
    if (s_with)
        (*str)++;
    if (e_with)
        (*str)[len - 2] = '\0';
    return (quote);
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
    while(arg[i] && !IS_QUOTE(arg[i]))
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

char    *parse_special_chars(char *str, int quote)
{
    int i;
    int j;

    i = 0;
    while (str[i])
    {
        if (str[i] == '\\')
        {
            j = i;
            if (quote)
            {
                str[j] = get_special_char(str[j + 1]);
                j++;
            }
            while (str[j])
            {
                str[j] = str[j + 1];
                j++;
            }
            str[j] = '\0';
        }
        i++;
    }
    return (str);
}

int			echo_builtin(t_cmds *cmd, char **env, int ret)
{
    int     i;
    int     n_flag;
    int     is_quote;

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
        is_quote = parse_quotes(&cmd->args[i]);
        if (ft_strchr(cmd->args[i] ,'\\'))
            cmd->args[i] = parse_special_chars(cmd->args[i], is_quote);
        if (is_quote != 1 && ft_strchr(cmd->args[i], '$'))
            cmd->args[i] = parse_variable(cmd->args[i], env, ret);
        echo_print(cmd->args, i);
        if (!cmd->args[i + 1] && !n_flag)
            ft_putchar_fd('\n', 1);
    }
    return (1);
}
