/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:19:16 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/14 12:42:21 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	quotes_checker(int *quote, int c)
{
	if (is_quote(c, 1) && !*quote)
		*quote = 1;
	else if (is_quote(c, 1) && *quote == 1)
		*quote = 0;
	if (is_quote(c, 2) && !*quote)
		*quote = 2;
	else if (is_quote(c, 2) && *quote == 2)
		*quote = 0;
}

char	*clear_quotes(char *str)
{
	int		j;
	int		i;
	int		quote;
	int		ignore;

	i = -1;
	quote = 0;
	ignore = 0;
	while (str[++i])
	{
		if (str[i] == '\\' || ignore)
			ignore = ignore ? 0 : 1;
		else if (((!quote && is_quote(str[i], 0)) ||
			(quote && is_quote(str[i], 0) == quote)) && !ignore)
		{
			quote = !quote ? is_quote(str[i], 0) : 0;
			j = i - 1;
			while (str[++j])
				str[j] = str[j + 1];
			i--;
		}
	}
	return (parse_special_chars(str));
}

char	*parse_special_chars(char *str)
{
	int	i;
	int	j;
	int	quote;

	i = -1;
	quote = 0;
	if (ft_strchr(str, '\\'))
	{
		while (str[++i])
		{
			if (is_quote(str[i], 0))
				quote = quote_activer(quote, str[i]);
			if (str[i] == '\\' && !quote)
			{
				j = i - 1;
				while (str[++j])
					str[j] = str[j + 1];
				str[j] = '\0';
			}
		}
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
