/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:23:39 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/14 10:16:56 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*parse_variable_name(char *str, int len, t_shell *shell)
{
	char	*tmp;
	char	*var;
	int		i;

	var = NULL;
	tmp = (char *)malloc(sizeof(char) * len);
	ft_strlcpy(tmp, str, len);
	if (tmp[0] == '?')
	{
		if (shell->ret == 0)
			var = ft_strdup("0");
		else
			var = ft_itoa(shell->ret);
	}
	else if ((i = ft_getenv(tmp, shell->env)) >= 0)
		var = shell->env[i] + ft_strlen(tmp) + 1;
	ft_del(tmp);
	if (var)
		return (var);
	return (NULL);
}

void	replace_in_string(char *src, char *dest, int *inc)
{
	*src = *dest;
	*inc = *inc + 1;
}

char	*replace_var_string(char *src, int i, char *var, int len)
{
	char	*tmp;
	int		j;
	int		c_src;
	int		c_var;
	int		tlen;

	j = 0;
	c_var = 0;
	c_src = 0;
	tlen = ft_strlen(src) + ft_strlen(var) - len;
	tmp = (char *)malloc(sizeof(char) * tlen);
	while (j < tlen - 1)
		if (j == i)
		{
			c_src = c_src + len + 1;
			while (var && var[c_var])
				replace_in_string(tmp + j, var + c_var++, &j);
			i = -1;
		}
		else
			replace_in_string(tmp + j, src + c_src++, &j);
	ft_del(src);
	tmp[j] = '\0';
	return (tmp);
}

char	*parse_env_var(char *str, t_shell *shell)
{
	int		i;
	int		quote;
	int		var;
	char	*tmp;

	i = -1;
	var = -1;
	quote = 0;
	while (str[++i])
	{
		if (quote != 1 && var != -1 && var_checker_pass(str[i + 1]))
		{
			tmp = parse_variable_name(str + var + 1, i - var + 1, shell);
			str = replace_var_string(str, var, tmp, i - var);
			i = var + ft_strlen(tmp) - 1;
			var = -1;
		}
		quotes_checker(&quote, str[i]);
		if (quote != 1 && str[i] == '$' && str[i - 1] != '\\')
			var = i;
	}
	return (str);
}

int		var_checker_pass(char c)
{
	if (!c || is_quote(c, 0) || !ft_isalnum(c) || c == ' '
		|| c == '$' || c == '}')
		return (1);
	return (0);
}
