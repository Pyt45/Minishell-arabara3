
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
        return (1);
    if (type == 0 && (c == '\'' || c == '\"'))
        return (1);
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

char    *parse_variables(char *arg, char **env, int ret)
{
    
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

static int	parse_quotes(char **str)
{
    int     s_with;
    int     e_with;
    int     quote;
    int     len;

    quote = 0;
    quote = is_quote(*str[0], 1);
    quote = is_quote(*str[0], 2) ? 2 : quote;
    s_with = is_quote(*str[0], 0);
	len = (int)ft_strlen(*str);
    e_with = is_quote((*str)[len - 1], 0);
    printf("S: %d | E:%d \n", s_with, e_with);
    if (s_with)
        (*str)++;
    if (e_with)
        (*str)[len - 2] = '\0';
    return (quote);
}

// static int  parse_quotes(char *str){
//     int     i;
//     int     pos;
//     int     quote;

//     i = 0;
//     quote = 0;
//     pos = 0;
//     while (str[i]){

//         i++;
//     }

// }

int     is_env_var(char *str)
{
    int     i;
    int     j;
    int     quote;
    int     var;
    int     clear;

    i = 0;
    j = 0;
    var = -1;
    quote = 0;
    clear = 0;
    if (ft_strchr(str, '$'))
    {
        while (str[i])
        {
            if (quote == 1 && str[i] == '\'')
                quote = 0;
            else if (str[i] == '\'' && !quote)
                quote = 1;

            if (str[i] == '\"' && !quote)
                quote = 2;
            else if (str[i] == '\"' && quote == 2)
                quote = 0;

            // if (str[i] == '$' && quote != 1)
            //     var = i;
            // if (str[i] == '\"')
            // {
            //     quote = 2;
            // }
            // if (str[i] == '\'')
            // {
            //     quote == 1;
            //     clear = 1;
            // }
            // if (j != 0 && var != 0){
            //     // replace in string with variable
            //     j = 0;
            // }
        }
    }
    return (0);
}

int			echo_builtin(t_cmds *cmd, char **env, int ret)
{
    int     i;
    int     n_flag;
    int     is_quote;

    i = 0;
    n_flag = 0;
    if (!cmd->args[1])
    {
        write(1, "\n", 1);
        return (0);
    }
    else if (cmd->args[1][0] == '-' && cmd->args[1][1] == 'n' && cmd->args[1][2] == '\0')
    {
        n_flag = 1;
        i++;
    }
    while (cmd->args[++i])
    {
        // is_quote = parse_quotes(&cmd->args[i]);
        // // printf("quotes: %d | arg: %s\n", is_quote, cmd->args[i]);
        // if (ft_strchr(cmd->args[i] ,'\\'))
        //     cmd->args[i] = parse_special_chars(cmd->args[i], is_quote);
        // if (is_quote != 1 && ft_strchr(cmd->args[i], '$'))
        //     cmd->args[i] = parse_variable(cmd->args[i], env, ret);
        if (is_env_var(cmd->args[i]))
            cmd->args[i] = parse_variables(cmd->args[i], env, ret);
        cmd->args[i] = clear_quotes(cmd->args[i]);
        echo_print(cmd->args, i);
        if (!cmd->args[i + 1] && !n_flag)
            ft_putchar_fd('\n', 1);
    }
    return (0);
}



/*

-nhello'$A'hello$A"$A"s$As


char    *parse_variable_name(char *str, int pos, int len){
    char    *var;

    var = (char *)malloc(sizeof(char) * len);
    strlcpy(var, str, len);
    return var;
}
int     is_env_var(char *str)
{
    int     i;
    int     j;
    int     quote;
    int     var;
    int     clear;
    char    *tmp;

    i = 0;
    j = 0;
    var = -1;
    quote = 0;
    clear = 0;
    if (strchr(str, '$'))
    {
        while (str[i])
        {   
            if (quote != 1 && var != -1 && (str[i + 1] == '\'' || str[i + 1] == '\"' || str[i + 1] == '\0'))
            {
                tmp = parse_variable_name(str + var + 1, 0, i - var + 1);
                printf("VAR: %s\n", tmp);
                str = replace_var_string(str, var, tmp, &i, i - var);
                printf("STR: %s\n", str);
                var = -1;
            }
            if (str[i] == '\'' && !quote)
                quote = 1;
            else if (quote == 1 && str[i] == '\'')
                quote = 0;

            if (str[i] == '\"' && !quote)
                quote = 2;
            else if (str[i] == '\"' && quote == 2)
            {
                quote = 0;
            }
            
            if (quote != 1 && str[i] == '$')
                var = i;
            // if (str[i] == '$' && quote != 1)
            //     var = i;
            // if (str[i] == '\"')
            // {
            //     quote = 2;
            // }
            // if (str[i] == '\'')
            // {
            //     quote == 1;
            //     clear = 1;
            // }
            // if (j != 0 && var != 0){
            //     // replace in string with variable
            //     j = 0;
            // }
            printf("C: %c | Q: %d | V: %d | POS: %d \n", str[i], quote, var, 0);
            i++;
        }
    }
    return (0);
}

int main(void)
{
    is_env_var("hello\"$PWD'\"$PWDs");

}



*/