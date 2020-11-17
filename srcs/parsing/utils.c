/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:10:41 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/17 19:01:26 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_cmds	*init_cmds(t_cmds *prev)
{
	t_cmds	*cmds;

	cmds = malloc(sizeof(t_cmds));
	cmds->start = 0;
	cmds->line = NULL;
	cmds->cmd = NULL;
	cmds->args = NULL;
	cmds->end = 0;
	cmds->p = 0;
	cmds->skip = 0;
	cmds->append = 0;
	cmds->ret = 0;
	cmds->prev = prev ? prev : NULL;
	cmds->next = prev && prev->next ? prev->next : NULL;
	return (cmds);
}

char	*get_cmd(char *str, int n)
{
	int		i;
	char	*cmd;
	int		quote;

	i = 0;
	quote = 0;
	while ((!ft_isprint(*str) || *str == ' ') && *str != '\0')
		str++;
	if (n == 0)
		n = ft_strlen(str);
	while (ft_isprint(str[i]) && i < n)
	{
		if (is_quote(str[i], 0))
			quote = quote_activer(quote, str[i]);
		if ((str[i] == ' ' || str[i] == ';' || str[i] == '|'
			|| str[i] == '>' || str[i] == '<') && !quote)
			break ;
		i++;
	}
	cmd = malloc(sizeof(char) * (++i));
	ft_strlcpy(cmd, str, i);
	if (*cmd)
		return (cmd);
	return (NULL);
}

char	**get_args(char *str, int n)
{
	int		i;
	char	*tmp;

	i = 0;
	while ((!ft_isprint(*str) || *str == ' ') && *str)
	{
		str++;
		i++;
	}
	if (*str)
	{
		tmp = ft_strdup(str);
		if (n - i >= 0)
			tmp[n - i] = '\0';
		if (*tmp)
			return (ft_split_quote(tmp, ' '));
	}
	return (NULL);
}

char	*replace_string(char *str, t_shell *shell)
{
	if (ft_strchr(str, '$'))
		return (parse_env_var(str, shell));
	return (str);
}

void	manage_redirections(t_cmds **cmds, int *i, char *tmp)
{
	if (!(*cmds)->prev)
		(*cmds)->start = 1;
	if (tmp[*i] == '>')
		(*cmds)->append = 1;
	else if (tmp[*i] == '<')
		(*cmds)->append = -1;
	if (tmp[*i + 1] == '>')
	{
		(*cmds)->append++;
		(*i)++;
	}
	else if (tmp[*i + 1] == '<')
	{
		(*cmds)->append--;
		(*i)++;
	}
}
