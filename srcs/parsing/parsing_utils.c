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
	if ((*cmds)->append > 2 || (*cmds)->append < -2)
		return (-1);
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
