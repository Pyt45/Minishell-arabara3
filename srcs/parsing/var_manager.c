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

	i = 0;
	tmp = NULL;
	if (g_ret == 1)
	{
		shell->ret = !shell->ret ? 1 : shell->ret;
		g_ret = 0;
	}
	prs->tmp[prs->c - prs->pos] = '\0';
	if (*(prs->tmp) == '?')
		tmp = ft_itoa(shell->ret);
	else if ((i = ft_getenv(prs->tmp, shell->env)) >= 0)
		tmp = ft_strdup(shell->env[i] + ft_strlen(prs->tmp) + 1);
	ft_del(prs->tmp);
	return (tmp);
}

char	*replace_var_str(t_parser *prs)
{
	int		tlen;
	char	*tmp;
	char	*src;

	prs->len = ft_strlen(prs->tmp);
	tlen = ft_strlen(prs->str) + prs->len - (prs->c - prs->pos) + 1;
	tmp = (char *)malloc(sizeof(char) * tlen);
	src = prs->str;
	while (*src)
	{
		prs->start = prs->move == prs->pos ? prs->move : prs->start;
		src = prs->move == prs->pos ? src + prs->c - prs->pos + 1 : src;
		prs->move = prs->move == prs->pos ? prs->move + prs->len : prs->move;
		*(tmp + prs->move) = *src;
		prs->move++;
		src++;
	}
	tmp[tlen] = '\0';
	src = prs->tmp;
	prs->move = -1;
	while (++prs->move < prs->len)
		*(tmp + prs->start + prs->move) = *(src + prs->move);
	ft_del(prs->str);
	return (tmp);
}

int		var_checker_pass(t_parser *prs, int start)
{
	if (start == 0 && prs->str[prs->c] == '$' &&
		((prs->quote && is_quote(prs->str[prs->c + 1], 0) == prs->quote)
		|| !prs->str[prs->c + 1]))
		return (0);
	if (start == 0 && prs->quote != 1 && prs->str[prs->c] == '$' &&
		!prs->ignore)
		return (1);
	if (start == 1 && prs->str[prs->c] == '$' && (prs->str[prs->c + 1] == '?'
		|| prs->str[prs->c + 1] == '_'))
		return (0);
	if (((!prs->str[prs->c + 1] || prs->str[prs->c] == '?' ||
		is_quote(prs->str[prs->c + 1], 0) ||
		!ft_isalnum(prs->str[prs->c + 1]) || prs->str[prs->c + 1] == ' '
		|| prs->str[prs->c + 1] == '$'))
		&& prs->quote != 1 && prs->tmp && start == 1)
		return (1);
	return (0);
}

char	*parse_env_var(char *str, t_shell *shell)
{
	t_parser	*prs;
	int		i;

	prs = init_parser(shell, str, 0);
	i = -1;
	while (prs->str[++i])
	{
		prs->c = i;
		if (prs->str[i] == '\\' && prs->quote != 1)
			prs->ignore = prs->ignore ? 0 : 1;
		if (var_checker_pass(prs, 0))
		{
			prs->tmp = ft_strdup(prs->str + i + 1);
			prs->pos = i;
		}
		if (var_checker_pass(prs, 1))
		{
			prs->tmp = get_variable_name(prs, shell);
			if (prs->quote != 1 || ft_strlen(prs->tmp))
				prs->str = replace_var_str(prs);
			ft_del(prs->tmp);
			prs->tmp = NULL;
		}
		prs->quote = quote_activer(prs->quote, prs->str[i]);
		if (prs->str[i] == '$' && prs->str[i + 1] == '\\')
			break ;
	}
	return (prs->str);
}

