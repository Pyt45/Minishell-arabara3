/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:19:16 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/18 11:45:18 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*str_quotes_replacer(char *str, int i)
{
	int		j;

	j = i - 1;
	while (str[++j])
		str[j] = str[j + 1];
	return (str);
}

char	*clear_quotes(char *str)
{
	int		i;
	int		quote;
	int		ignore;

	i = -1;
	quote = 0;
	ignore = 0;
	str = ft_strtrim(str, "\t\v\n\f\r");
	while (str[++i])
	{
		if ((str[i] == '\\' || ignore) && quote != 1)
			ignore = ignore ? 0 : 1;
		else if (((!quote && is_quote(str[i], 0)) ||
			(quote && is_quote(str[i], 0) == quote)))
		{
			quote = !quote ? is_quote(str[i], 0) : 0;
			str = str_quotes_replacer(str, i--);
		}
		if (ignore && (((str[i + 1] == '$' || str[i + 1] == '"'
			|| str[i + 1] == '\\') && quote == 2) || !quote))
			str = str_quotes_replacer(str, i--);
	}
	return (str);
}

int		quote_activer(int quote, char c)
{
	if (!quote)
		return (is_quote(c, 0));
	else if (quote == is_quote(c, 0))
		return (0);
	else
		return (quote);
}

int		is_quote(char c, int type)
{
	if (type == 1 && c == '\'')
		return (1);
	if (type == 2 && c == '\"')
		return (2);
	if (type == 0 && (c == '\'' || c == '\"'))
		return (c == '\'' ? 1 : 2);
	return (0);
}

int		create_cmd_line(t_cmds **cmds, char *tmp, int start, int end)
{
	if ((*cmds)->line)
	{
		(*cmds)->next = init_cmds(*cmds);
		(*cmds) = (*cmds)->next;
	}
	(*cmds)->line = ft_substr(tmp, start, end - start + 1);
	if (!validate_line((*cmds)->line))
		return (-1);
	return (end + 1);
}
