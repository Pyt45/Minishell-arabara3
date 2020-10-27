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
// "sd"
static int		count_strings(char *str)
{
	int i;
	int j;
	int start;
	int quote;

	i = 0;
	j = 0;
	quote = 0;
	start = 0;
	while (str[i])
	{
		if (is_quote(str[i], 0))
		{
			// idk what this is for
			// if (start)
			// 	j++;
			quote = quote_activer(quote, str[i]);
		}
		else if (str[i] == ' ' && !quote && str[i - 1] != '\\')
		{
			if (start == 1)
			{
				j++;
				start = 0;
			}
		}
		else if (str[i] != ' ')
			start = 1;
		i++;
	}
	if (start)
		j++;
	return (j);
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
		else if (str[i] != x && j == -1)
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
	// write_to_file(" LEN", ft_itoa(len), 1);
	r = (char **)malloc(sizeof(char *) * (len + 1));
	if (!r)
		return (0);
	r = ft_make_splits(r, (char *)s, c);
	if (!r)
		return (NULL);
	r[len] = 0;
	return (r);
}