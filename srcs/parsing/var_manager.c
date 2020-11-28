/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:23:39 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/19 10:01:43 by zlayine          ###   ########.fr       */
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
	if (g_ret == 1)
	{
		shell->ret = !shell->ret ? 1 : shell->ret;
		g_ret = 0;
	}
	if (tmp[0] == '?')
		var = ft_itoa(shell->ret);
	else if ((i = ft_getenv(tmp, shell->env)) >= 0)
		var = ft_strdup(shell->env[i] + ft_strlen(tmp) + 1);
	ft_del(tmp);
	return (var ? var : NULL);
}

char	*get_variable_name(t_parser *prs, t_shell *shell)
{
	int	i;
	char	*tmp;

	tmp = NULL;
	if (g_ret == 1)
	{
		shell->ret = !shell->ret ? 1 : shell->ret;
		g_ret = 0;
	}
	prs->tmp[prs->c - prs->pos + 1] = '\0';
	if (*(prs->tmp + 1) == '?')
		tmp = ft_itoa(shell->ret);
	else if ((i = ft_getenv(prs->tmp, shell->env)) >= 0)
		tmp = ft_strdup(shell->env[i] + ft_strlen(prs->tmp) + 1);
	ft_del(prs->tmp);
	return (tmp);
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

char	*replace_var_str(t_parser *prs)
{
	int		tlen;
	char	*tmp;

	tlen = ft_strlen(prs->str) + ft_strlen(prs->tmp) - (prs->c - prs->pos);
	tmp = (char *)malloc(sizeof(char) * tlen);
	while (--tlen)
	{
		if (tlen == prs->pos)
		{

		}
		else
			replace_in_string(tmp + tlen, src + c_src, &j);
	}
}


char	*parse_env_var(char *str, t_shell *shell)
{
	t_parser	*prs;
	int		i;
	int		quote;
	int		var;
	char	*tmp;

	prs = init_parser(shell, str, 0);
	i = -1;
	var = -1;
	while (prs->str[++i])
	{
		prs->c = i;
		if (prs->str[i] == '\\' && prs->quote != 1)
			prs->ignore = prs->ignore ? 0 : 1;
		if (prs->quote != 1 && prs->str[i] == '&' && !prs->ignore)
		{
			prs->tmp = ft_strdup(prs->str + i);
			prs->pos = i;
		}
		if (prs->quote != 1 && prs->tmp && var_checker_pass(str[i + 1]))
		{
			prs->tmp = get_variable_name(prs, shell);
			if (!quote || ft_strlen(prs->tmp))
				prs->str = replace_var_str(prs);
			prs->tmp = NULL;
		}

		if (quote != 1 && var != -1 && var_checker_pass(str[i + 1]))
		{
			tmp = parse_variable_name(str + var + 1, i - var + 1, shell);
			if (!quote || ft_strlen(tmp) || i - var > 0)
				str = replace_var_string(str, var, tmp, i - var);
			i = ft_strlen(tmp) || i - var > 0 ? var + ft_strlen(tmp) - 1 : i;
			ft_del(tmp);
			var = -1;
		}
		quotes_checker(&prs->quote, prs->str);
		if (str[i] == '$' && str[i + 1] == '\\')
			break ;
	}
	return (str);
}

int		var_checker_pass(char c)
{
	if (c == '?' || c == '_')
		return (0);
	if ((!c || is_quote(c, 0) || !ft_isalnum(c) || c == ' ' || c == '$'))
		return (1);
	return (0);
}
