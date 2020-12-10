/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 11:51:00 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/17 17:54:14 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char			**free_list_quote(char **split)
{
	int i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
	return (NULL);
}

static int		count_strings(char *str)
{
	int i;
	int j;
	int start;
	int quote;
	int	ignore;

	i = -1;
	j = 0;
	quote = 0;
	start = 0;
	ignore = 0;
	while (str[++i])
	{
		if ((str[i] == '\\') && quote != 1)
			ignore = ignore ? 0 : 1;
		if (is_quote(str[i], 0) && !ignore)
			quote = quote_activer(quote, str[i]);
		j = str[i] == ' ' && !quote && !ignore && start ? j + 1 : j;
		start = str[i] == ' ' && !quote && !ignore && start ? 0 : start;
		start = str[i] != ' ' && str[i] != '\t' ? 1 : start;
		ignore = ignore && str[i] != '\\' ? 0 : ignore;
	}
	return (start ? j + 1 : j);
}

static char		**ft_make_splits(char **split, char *str, char x)
{
	int i;
	int j;
	int quote;
	int c;

	quote = 0;
	i = -1;
	j = -1;
	c = 0;
	while (str[++i])
	{
		if (is_quote(str[i], 0))
			quote = quote_activer(quote, str[i]);
		if (str[i] == x && !quote && j != -1 && str[i - 1] != '\\')
		{
			if (!(split[c++] = clear_quotes(ft_substr(str, j, i - j))))
				return (free_list_quote(split));
			j = -1;
		}
		else if (str[i] != x && str[i] != '\t' && j == -1)
			j = i;
	}
	if (j != -1 && !(split[c] = clear_quotes(ft_substr(str, j, i - j))))
		return (free_list_quote(split));
	return (split);
}

char			**ft_split_quote(char const *s, char c)
{
	char	**r;
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	len = count_strings((char *)s);
	r = NULL;
	r = (char **)malloc(sizeof(char *) * (len + 1));
	if (!r)
		return (0);
	r = ft_make_splits(r, (char *)s, c);
	ft_del((char *)s);
	if (!r)
		return (NULL);
	r[len] = 0;
	return (r);
}

t_parser		*init_parser(t_shell *shell, char *line, int var)
{
	t_parser *parser;

	parser = malloc(sizeof(t_parser));
	parser->pos = 0;
	parser->start = 0;
	parser->end = 0;
	parser->c = 0;
	parser->move = 0;
	parser->ignore = 0;
	parser->quote = 0;
	parser->tmp = NULL;
	if (var == 1)
		parser->str = replace_string(ft_strdup(line), shell);
	else
		parser->str = ft_strdup(line);
	if (var == 3)
		ft_del(line);
	if (var == 4)
		parser->str = ft_strtrim(parser->str, " \t");
	return (parser);
}
