
#include "../includes/shell.h"

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
    int     is_s_quote;
    int     len;

    is_s_quote = IS_S_QUOTE(*str[0]);
    s_with = IS_QUOTE(*str[0]);
	len = (int)ft_strlen(*str);
    e_with = IS_QUOTE((*str)[len - 1]);
    if (s_with)
        (*str)++;
    if (e_with)
        (*str)[len - 2] = '\0';
    if (is_s_quote)
        return (1);
    return (0);
}

static void echo_print(char **str, int pos)
{
    ft_putstr_fd(str[pos], 1);
	if (str[pos + 1])
		ft_putchar_fd(' ', 1);
}

static char	*parse_variable(char *arg, char **env)
{
    int i;
    int pos;
    char *path;

    i = 0;
    while(arg[i] && !IS_QUOTE(arg[i]))
    {
        if (arg[i] == '$')
            pos = i + 1;
        i++;
    }
    if ((i = ft_getenv(arg + pos, env)) >= 0)
		path = env[i] + ft_strlen(arg + pos) + 1;
    arg[pos - 1] = '\0';
    return (ft_strjoin(arg, path));
}

int			echo_builtin(t_cmds *cmd, char **env)
{
    int     i;
    int     n_flag;
    int     is_s_quote;

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
        is_s_quote = parse_quotes(&cmd->args[i]);
        if (!is_s_quote && ft_strchr(cmd->args[i], '$'))
            cmd->args[i] = parse_variable(cmd->args[i], env);
        echo_print(cmd->args, i);
        if (!cmd->args[i + 1] && !n_flag)
            ft_putchar_fd('\n', 1);
    }
    return (1);
}
