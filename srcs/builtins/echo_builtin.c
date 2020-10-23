
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

char    *parse_special_chars(char *str)
{
    int i;
    int j;
	int	quote;

    i = 0;
	quote = 0;
    while (str[i])
    {
		if (is_quote(str[i], 0))
			quote = 1;
        if (str[i] == '\\' && !quote)
        {
            j = i;
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

char    *parse_variable_name(char *str, int len, t_shell *shell){
    char    *tmp;
    char    *var;
    int i;

    var = NULL;
	if (*str == '{')
	{
		len--;
		str++;
	}
    tmp = (char *)malloc(sizeof(char) * len);
    strlcpy(tmp, str, len);
    if (tmp[0] == '?')
    {
        if (shell->ret == 0)
            var = ft_strdup("0");
        else
            var = ft_itoa(shell->ret);
    }
    else if ((i = ft_getenv(tmp, shell->env)) >= 0)
		var = shell->env[i] + ft_strlen(tmp) + 1;
    free(tmp);
    if (var)
        return var;
    return NULL;
}

char    *replace_var_string(char *src, int i, char *var, int *pos, int len)
{
    char    *tmp;
    int     j;
    int     tlen;
	int		varlen;

    j = 0;
	varlen = ft_strlen(var);
	len = src[i] == '{' ? len + 2 : len; 
	i = src[i] == '{' ? i - 1 : i;
    tlen = ft_strlen(src) + varlen - len;
    tmp = (char *)malloc(sizeof(char) * tlen);
    while (j < tlen){
		if (j == i)
            src = src + len + 1;
        if (j == i && var)
        {
            while (*var) {
                tmp[j] = *var;
                var++;
                j++;
            }
            // src = src + len + 1;
        } else {
            // if (j == i && !var)
                // src = src + len + 1;
            tmp[j] = *src;
            src++;
            j++;
        }
    }
    tmp[j] = '\0';
    *pos = i + varlen;
    return tmp;
}

char    *clear_quotes(char *str)
{
    int     j;
    int     i;
	int		quote;
	int		forbidden;

    i = 0;
	quote = 0;
	forbidden = 0;
    while (str[i]){
        if ((!quote && is_quote(str[i], 0)) || (quote && is_quote(str[i], 0) == quote)){
			if (!quote && str[i + 1] == ' ')
			{
				forbidden = 1;
			}
			quote = !quote ? is_quote(str[i], 0) : 0;
			j = i;
			while (str[j])
			{
				// if (!forbidden)
				str[j] = str[j + 1];
				j++;
			}
			// if (!forbidden)
			i--;
        }
        i++;
    }
    return (str);
}

char     *parse_env_var(char *str, t_shell *shell)
{
    int     i;
    int     quote;
    int     var;
    char    *tmp;

    i = -1;
    var = -1;
    quote = 0;
	while (str[++i])
	{   
		if (quote != 1 && var != -1 && (is_quote(str[i + 1], 0) || !ft_isalnum(str[i + 1]) || !str[i + 1] || str[i + 1] == ' ' || str[i + 1] == '$' || str[i + 1] == '}'))
		{
			tmp = parse_variable_name(str + var + 1, i - var + 1, shell);
			str = replace_var_string(str, var, tmp, &i, i - var);
			var = -1;
		}
		if (is_quote(str[i], 1) && !quote)
			quote = 1;
		else if (is_quote(str[i], 1) && quote == 1)
			quote = 0;
		if (is_quote(str[i], 2) && !quote)
			quote = 2;
		else if (is_quote(str[i], 2)&& quote == 2)
			quote = 0;
		if (quote != 1 && str[i] == '$')
		{
			if (str[i + 1] == '{')
				i++;
			var = i;
		}
	}
    return (str);
}

char	*replace_string(char *str, t_shell *shell)
{
	if (ft_strchr(str, '$'))
	    str = parse_env_var(str, shell);
	// if (ft_strchr(str ,'\\'))
	// 	str = parse_special_chars(str);
	// return (clear_quotes(str));
	return (str);
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
