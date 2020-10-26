/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:23:39 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/26 18:42:08 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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

void	replace_in_string(char *src, char **dest, int *inc)
{
	*src = **dest;
	(*dest)++;
	*inc = *inc + 1; 
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
        {
			src = src + len + 1;
            while (var && *var)
				replace_in_string(tmp + j, &var, &j);
			i = -1;
        } else 
			replace_in_string(tmp + j, &src, &j);
    }
    tmp[j] = '\0';
    *pos = i + varlen;
    return tmp;
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
		if (quote != 1 && var != -1 && var_checker_pass(str[i + 1]))
		{
			tmp = parse_variable_name(str + var + 1, i - var + 1, shell);
			str = replace_var_string(str, var, tmp, &i, i - var);
			var = -1;
		}
		quotes_checker(&quote, str[i]);
		if (quote != 1 && str[i] == '$')
		{
			if (str[i + 1] == '{')
				i++;
			var = i;
		}
	}
    return (str);
}