/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:10:41 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/19 09:32:03 by zlayine          ###   ########.fr       */
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
	t_parser	*prs;
	
	i = 0;
	prs = init_parser(NULL, str, 0);
	while ((prs->str[i] == ' ') && prs->str[i])
		i++;
	n = n == 0 ? ft_strlen(prs->str + i) : n;
	while (prs->str[i] && i < n)
	{
		if (prs->str[i] == '\\' && prs->quote != 1)
			prs->ignore = prs->ignore ? 0 : 1;
		if (is_quote(prs->str[i], 0) && !prs->ignore)
			prs->quote = quote_activer(prs->quote, prs->str[i]);
		if (!prs->ignore && !prs->quote && ft_strchr(" ;|><", prs->str[i]))
			break ;
		prs->ignore = prs->ignore && prs->str[i] != '\\' ? 0 : prs->ignore;
		i++;
	}
	while (ft_strchr(" \t", prs->str[i]) && prs->str[i])
		i++;
	cmd = malloc(sizeof(char) * (++i));
	ft_strlcpy(cmd, prs->str, i);
	ft_del(prs->str);
	ft_del(prs);
	return (*cmd ? cmd : NULL);
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
		ft_del(tmp);
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
	int j;

	if (!(*cmds)->prev)
		(*cmds)->start = 1;
	if (tmp[*i] == '>')
		(*cmds)->append = 1;
	else if (tmp[*i] == '<')
		(*cmds)->append = -1;
	while ((*cmds)->append > 0 && tmp[*i + 1] == '>')
	{
		(*cmds)->append++;
		(*i)++;
	}
	while ((*cmds)->append < 0 && tmp[*i + 1] == '<')
	{
		(*cmds)->append--;
		(*i)++;
	}
	j = *i;
	while (tmp[++j])
		if ((tmp[j] == '>' && (*cmds)->append < 0) || (tmp[j] == '<' &&
			(*cmds)->append > 0) || tmp[j] == '<' || tmp[j] == '>')
			(*cmds)->append = 20;
		else if (ft_isprint(tmp[j]) && tmp[j] != ' ')
			break ;
}
