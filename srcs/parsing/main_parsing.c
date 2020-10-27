/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:10:44 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/27 09:28:07 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int        parse_pipes(t_cmds **cmds, int i, int pos,char *tmp){

    (*cmds)->cmd = get_cmd(tmp + pos, i - pos);
    (*cmds)->args = get_args(tmp + pos, i - pos);
    // debug_cmd(*cmds, i, pos, tmp[i]);
	if (!(*cmds)->args)
		return (-1);
    if (!(*cmds)->prev)
        (*cmds)->start = 1;
	// this to manage syntax error and segfault (maybe remove the end of string check)
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

int        parse_semicolons(t_cmds **cmds, int i, int pos,char *tmp)
{
    int j;

    j = 0;
    if (tmp[i + 1] == '\0' && tmp[i] != ';')
        j = 1;
    (*cmds)->cmd = get_cmd(tmp + pos, i - pos + j);
    (*cmds)->args = get_args(tmp + pos, i - pos + j);
    debug_cmd((*cmds), i, pos, tmp[i]);
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

int        parse_redirections(t_cmds **cmds, int *i, int pos,char *tmp){
    (*cmds)->cmd = get_cmd(tmp + pos, *i - pos);
    (*cmds)->args = get_args(tmp + pos, *i - pos);
    // debug_cmd(*cmds, *i, pos, tmp[*i]);
	if (!(*cmds)->args ||!(*cmds)->cmd)
		return (-1);
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
	return (pos);
}

t_shell     *parse_commands(t_shell *shell)
{
    t_cmds      *cmds;
    int         i;
    int         pos;
    char        *tmp;
	int			quote;
    
    pos = 0;
    i = -1;
	quote = 0;
	tmp = replace_string(shell->line, shell);
    cmds = init_cmds(NULL);
    shell->cmds = cmds;
    while (tmp[++i] && pos != -1)
    {
		if (is_quote(tmp[i], 0))
			quote = quote_activer(quote, tmp[i]);
		if (!quote)
			pos = manage_parsing(&cmds, &i, pos, tmp);
		shell->parse_err = pos;
    }
	if (quote)
		shell->parse_err = -1;
    return (shell);
}

