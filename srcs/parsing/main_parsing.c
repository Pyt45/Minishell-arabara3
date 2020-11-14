/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:10:44 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/14 14:46:38 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		parse_pipes(t_cmds **cmds, int i, int pos, char *tmp)
{
	(*cmds)->cmd = get_cmd(tmp + pos, i - pos);
	(*cmds)->args = get_args(tmp + pos, i - pos);
	if (!(*cmds)->args)
		return (-1);
	if (!(*cmds)->prev)
		(*cmds)->start = 1;
	if (tmp[i + 1] != '\0')
	{
		(*cmds)->p = 1;
		(*cmds)->next = init_cmds(*cmds);
		(*cmds) = (*cmds)->next;
	}
	else
		return (-1);
	pos = i + 1;
	return (pos);
}

int		parse_semicolons(t_cmds **cmds, int i, int pos, char *tmp)
{
	int	j;

	j = 0;
	if (tmp[i + 1] == '\0' && tmp[i] != ';')
		j = 1;
	(*cmds)->cmd = get_cmd(tmp + pos, i - pos + j);
	(*cmds)->args = get_args(tmp + pos, i - pos + j);
	if (!(*cmds)->args)
		return (-1);
	if (!(*cmds)->prev)
		(*cmds)->start = 1;
	(*cmds)->end = 1;
	if (tmp[i + 1] != '\0')
	{
		(*cmds)->next = init_cmds(*cmds);
		(*cmds) = (*cmds)->next;
	}
	pos = i + 1;
	return (pos);
}

int		parse_redirections(t_cmds **cmds, int *i, int pos, char *tmp)
{
	(*cmds)->cmd = get_cmd(tmp + pos, *i - pos);
	(*cmds)->args = get_args(tmp + pos, *i - pos);
	manage_redirections(cmds, i, tmp);
	(*cmds)->next = init_cmds((*cmds));
	(*cmds) = (*cmds)->next;
	if (!tmp[*i + 1])
		return (-1);
	pos = *i + 1;
	return (pos);
}

int		manage_parsing(t_cmds **cmds, int *i, int pos, char *tmp)
{
	if (tmp[*i] == '|')
		pos = parse_pipes(cmds, *i, pos, tmp);
	else if (tmp[*i] == '>' || tmp[*i] == '<')
		pos = parse_redirections(cmds, i, pos, tmp);
	else if (tmp[*i] == ';' || tmp[*i + 1] == '\0')
		pos = parse_semicolons(cmds, *i, pos, tmp);
	if ((*cmds)->prev && (*cmds)->prev->append < 0)
		(*cmds)->skip = 1;
	return (pos);
}

t_shell	*parse_commands(t_shell *shell)
{
	t_cmds		*cmds;
	t_parser	*parser;
	int			i;

	parser = init_parser(shell);
	cmds = init_cmds(NULL);
	shell->cmds = cmds;
	i = -1;
	while (parser->str[++i] && parser->pos != -1)
	{
		if (parser->str[i] == '\\' || parser->ignore)
			parser->ignore = parser->ignore ? 0 : 1;
		else if (!parser->ignore)
		{
			if (is_quote(parser->str[i], 0) && !parser->ignore)
				parser->quote = quote_activer(parser->quote, parser->str[i]);
			if (!parser->quote)
				parser->pos = manage_parsing(&cmds, &i,
					parser->pos, parser->str);
		}
		shell->parse_err = parser->pos;
	}
	shell->parse_err = parser->quote ? -1 : shell->parse_err;
	ft_del(parser->str);
	ft_del(parser);
	return (shell);
}
