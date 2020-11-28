/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:10:44 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/19 09:16:52 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int				validate_line(char *str)
{
	int		i;
	char	**tmp;
	int		pos;
	int		j;

	i = ft_strlen(str);
	while (str[--i] == ' ')
		str[i] = '\0';
	i = -1;
	pos = 0;
	while (str[++i])
		if ((str[i] == '>' || str[i] == '<') && !str[i + 1])
			return (0);
		else if (str[i] == ';' || str[i] == '|')
		{
			tmp = get_args(str, ft_strlen(str + pos) - 1);
			if (!tmp)
				return (0);
			pos = i;
			j = -1;
			while (tmp[++j])
				ft_del(tmp[j]);
			ft_del(tmp);
		}
	return (1);
}

int				check_parsing(t_shell *shell)
{
	if (shell->parse_err == -1)
	{
		print_error("syntax error", 0, 0);
		shell->ret = 258;
		return (0);
	}
	else if (shell->parse_err == -2)
		return (0);
	return (1);
}

t_shell			*parse_commands(t_shell *shell)
{
	t_cmds		*cmds;
	t_parser	*prs;
	int			i;

	prs = init_parser(shell, shell->line, 0);
	cmds = init_cmds(NULL);
	shell->cmds = cmds;
	i = -1;
	while (prs->str[++i] && prs->pos != -1)
	{
		if (prs->str[i] == '\\' && prs->quote != 1)
			prs->ignore = prs->ignore ? 0 : 1;
		if (is_quote(prs->str[i], 0) && !prs->ignore)
			prs->quote = quote_activer(prs->quote, prs->str[i]);
		if (!prs->quote && (prs->str[i] == ';' || prs->str[i + 1] == '\0'))
			prs->pos = create_cmd_line(&cmds, prs->str, prs->pos, i);
		prs->ignore = prs->ignore && prs->str[i] != '\\' ? 0 : prs->ignore;
	}
	shell->parse_err = prs->quote || prs->ignore ? -1 : prs->pos;
	ft_del(prs->str);
	ft_del(prs);
	return (shell);
}

t_cmds			*parse_command(t_shell *shell, t_cmds *cmds)
{
	t_parser	*prs;
	int			i;

	prs = init_parser(shell, cmds->line, 1);
	i = -1;
	while (prs->str[++i] && prs->pos != -1)
	{
		prs->c = i;
		if (prs->str[i] == '\\' && prs->quote != 1)
			prs->ignore = prs->ignore ? 0 : 1;
		if (is_quote(prs->str[i], 0) && !prs->ignore)
			prs->quote = quote_activer(prs->quote, prs->str[i]);
		if (!prs->quote)
			i = manage_parsing(&cmds, prs);
		prs->ignore = prs->ignore && prs->str[i] != '\\' ? 0 : prs->ignore;
	}
	shell->parse_err = prs->quote || prs->ignore ? -1 : prs->pos;
	ft_del(prs->str);
	ft_del(prs);
	return (cmds);
}
