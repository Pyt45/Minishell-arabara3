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

static int		count_strings(char *str, char c)
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
			if (quote)
				j++;
			quote = !quote ? 1 : 0;
		}
		else if (str[i] == c && !quote)
		{
			if (start == 1)
			{
				j++;
				start = 0;
			}
		}
		else if (str[i] != c)
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
			quote = !quote ? 1 : 0;
		if (str[i] == x && !quote && j != -1)
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
	printf("***************TOTAL: %d %s\n", c, split[c]);
	return (split);
}

char			**ft_split_quote(char const *s, char c)
{
	char	**r;
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	len = count_strings((char *)s, c);
	printf("%d\n", len);
	r = (char **)malloc(sizeof(char *) * (len + 1));
	if (!r)
		return (0);
	r = ft_make_splits(r, (char *)s, c);
	if (!r)
		return (NULL);
	r[len] = 0;
	return (r);
}