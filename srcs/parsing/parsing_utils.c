/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 19:13:25 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/20 10:10:24 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		parse_pipes(t_cmds **cmds, t_parser *prs)
{
	(*cmds)->cmd = get_cmd(prs->str + prs->pos, prs->len);
	(*cmds)->args = get_args(prs->str + prs->pos, prs->len);
	if (!(*cmds)->args)
		return (-1);
	if (!(*cmds)->prev)
		(*cmds)->start = 1;
	if (prs->str[prs->c + 1] != '\0')
	{
		(*cmds)->p = 1;
		(*cmds)->next = init_cmds(*cmds);
		(*cmds) = (*cmds)->next;
	}
	else
		return (-1);
	return (prs->c + 1);
}

int		parse_semicolons(t_cmds **cmds, t_parser *prs)
{
	int	j;

	j = 0;
	if (prs->str[prs->c + 1] == '\0' && prs->str[prs->c] != ';')
		j = 1;
	prs->len += j;
	(*cmds)->cmd = get_cmd(prs->str + prs->pos, prs->len);
	(*cmds)->args = get_args(prs->str + prs->pos, prs->len);
	if (!(*cmds)->args)
		return (-1);
	if (!(*cmds)->prev)
		(*cmds)->start = 1;
	(*cmds)->end = 1;
	if (prs->str[prs->c + 1])
	{
		(*cmds)->next = init_cmds(*cmds);
		(*cmds) = (*cmds)->next;
	}
	return (prs->c + 1);
}

int		parse_redirections(t_cmds **cmds, t_parser *prs)
{
	(*cmds)->cmd = get_cmd(prs->str + prs->pos, prs->len);
	(*cmds)->args = get_args(prs->str + prs->pos, prs->len);
	manage_redirections(cmds, &prs->c, prs->str);
	if ((*cmds)->append > 2 || (*cmds)->append < -2)
		return (-1);
	(*cmds)->next = init_cmds((*cmds));
	(*cmds) = (*cmds)->next;
	if (!prs->str[prs->c + 1])
		return (-1);
	return (prs->c + 1);
}

int		parse_full(t_cmds **cmds, t_parser *prs)
{
	prs->len += 1;
	(*cmds)->cmd = get_cmd(prs->str + prs->pos, prs->len);
	(*cmds)->args = get_args(prs->str + prs->pos, prs->len);
	if (!(*cmds)->args)
		return (-1);
	if (!(*cmds)->prev)
		(*cmds)->start = 1;
	(*cmds)->end = 1;
	if (prs->str[prs->c + 1])
	{
		(*cmds)->next = init_cmds(*cmds);
		(*cmds) = (*cmds)->next;
	}
	return (prs->c + 1);
}

int		manage_parsing(t_cmds **cmds, t_parser *prs)
{
	prs->len = prs->c - prs->pos;
	if (prs->str[prs->c] == '|' && !prs->ignore)
		prs->pos = parse_pipes(cmds, prs);
	else if ((prs->str[prs->c] == '>' || prs->str[prs->c] == '<') &&
		!prs->ignore)
		prs->pos = parse_redirections(cmds, prs);
	else if ((prs->str[prs->c] == ';' && !prs->ignore) ||
		prs->str[prs->c + 1] == '\0')
		prs->pos = parse_semicolons(cmds, prs);
	else if (prs->str[prs->c + 1] == '\0')
		prs->pos = parse_full(cmds, prs);
	if ((*cmds)->prev && (*cmds)->prev->append < 0)
		(*cmds)->skip = 1;
	return (prs->c);
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
