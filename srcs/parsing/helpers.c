/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:19:16 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/27 09:36:08 by zlayine          ###   ########.fr       */
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
	else if (is_quote(c, 2)&& *quote == 2)
		*quote = 0;
}

int		var_checker_pass(char c)
{
	if (!c || is_quote(c, 0) || !ft_isalnum(c) || c == ' '
		||  c == '$' || c == '}')
		return (1);
	return (0);
}

char    *clear_quotes(char *str)
{
    int     j;
    int     i;
	int		quote;

    i = 0;
	quote = 0;
	str = parse_special_chars(str);
    while (str[i]){
        if ((!quote && is_quote(str[i], 0)) || (quote && is_quote(str[i], 0) == quote)){
			quote = !quote ? is_quote(str[i], 0) : 0;
			j = i;
			while (str[j])
			{
				str[j] = str[j + 1];
				j++;
			}
			i--;
        }
        i++;
    }
    return (str);
}

char    *parse_special_chars(char *str)
{
    int i;
    int j;
	int	quote;

    i = 0;
	quote = 0;
	if (ft_strchr(str, '\\'))
	{
		while (str[i])
		{
			if (is_quote(str[i], 0))
				quote = quote_activer(quote, str[i]);
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
	}
    return (str);
}

int			quote_activer(int quote, char c)
{
	return !quote ? is_quote(c, 0) : quote == is_quote(c, 0) ? 0 : quote;
}
