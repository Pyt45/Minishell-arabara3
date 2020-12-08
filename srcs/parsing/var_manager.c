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

char	*get_variable_name(t_parser *prs, t_shell *shell)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (g_ret == 1)
	{
		shell->ret = !shell->ret ? 1 : shell->ret;
		g_ret = 0;
	}
	prs->tmp[prs->c - prs->pos] = '\0';
	if (prs->tmp[0] == '?')
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

	if (prs->quote == 1 && !ft_strlen(prs->tmp))
		return (prs->str);
	prs->len = ft_strlen(prs->tmp);
	tlen = ft_strlen(prs->str) + prs->len - (prs->c - prs->pos + 1) + 1;
	tmp = (char *)malloc(sizeof(char) * tlen);
	src = prs->str;
	while (*src)
	{
		src = prs->move == prs->pos ? src + prs->c - prs->pos + 1 : src;
		prs->move = prs->move == prs->pos ? prs->move + prs->len : prs->move;
		*(tmp + prs->move) = *src;
		src = *src ? src + 1 : src;
		prs->move++;
	}
	prs->move = -1;
	prs->c = prs->len ? prs->pos + prs->len - 1 : prs->pos;
	while (++prs->move < prs->len)
		*(tmp + prs->pos + prs->move) = *(prs->tmp + prs->move);
	tmp[tlen - 1] = '\0';
	ft_del(prs->str);
	return (tmp);
}

int		var_checker_pass(t_parser *prs, int start)
{
	prs->move = 0;
	if (start == 0 && prs->str[prs->c] == '$' &&
		((prs->quote && is_quote(prs->str[prs->c + 1], 0) == prs->quote)
		|| !prs->str[prs->c + 1] || prs->str[prs->c + 1] == '/' ||
			prs->str[prs->c + 1] == '\\'))
		return (0);
	if (start == 0 && prs->quote != 1 && prs->str[prs->c] == '$' &&
		!prs->ignore)
	{
		prs->pos = prs->c;
		return (1);
	}
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

	prs = init_parser(shell, str, 3);
	while (prs->str[prs->c])
	{
		if (prs->str[prs->c] == '\\' && prs->quote != 1)
			prs->ignore = prs->ignore ? 0 : 1;
		if (is_quote(prs->str[prs->c], 0) && !prs->ignore)
			prs->quote = quote_activer(prs->quote, prs->str[prs->c]);
		if (!prs->ignore && var_checker_pass(prs, 0))
			prs->tmp = ft_strdup(prs->str + prs->c + 1);
		if (!prs->ignore && var_checker_pass(prs, 1))
		{
			prs->tmp = get_variable_name(prs, shell);
			prs->str = replace_var_str(prs);
			ft_del(prs->tmp);
			prs->tmp = NULL;
		}
		prs->ignore = prs->ignore && prs->str[prs->c] != '\\' ? 0 : prs->ignore;
		prs->c = prs->str[prs->c] ? prs->c + 1 : prs->c;
	}
	str = prs->str;
	ft_del(prs);
	return (str);
}
