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
	t_parser	*parser;
	int			i;

	parser = init_parser(shell, shell->line, 0);
	cmds = init_cmds(NULL);
	shell->cmds = cmds;
	i = -1;
	while (parser->str[++i] && parser->pos != -1)
	{
		if (parser->str[i] == '\\' && parser->quote != 1)
			parser->ignore = parser->ignore ? 0 : 1;
		if (is_quote(parser->str[i], 0) && !parser->ignore)
			parser->quote = quote_activer(parser->quote, parser->str[i]);
		if (!parser->quote && (parser->str[i] == ';' ||
			parser->str[i + 1] == '\0'))
			parser->pos = create_cmd_line(&cmds,
				parser->str, parser->pos, i);
		if (parser->ignore && parser->str[i] != '\\')
			parser->ignore = 0;
	}
	shell->parse_err = parser->quote || parser->ignore ? -1 : parser->pos;
	ft_del(parser->str);
	ft_del(parser);
	return (shell);
}

t_cmds			*parse_command(t_shell *shell, t_cmds *cmds)
{
	t_parser	*parser;
	int			i;

	parser = init_parser(shell, cmds->line, 1);
	i = -1;
	while (parser->str[++i] && parser->pos != -1)
	{
		if (parser->str[i] == '\\' && parser->quote != 1)
			parser->ignore = parser->ignore ? 0 : 1;
		if (is_quote(parser->str[i], 0) && !parser->ignore)
			parser->quote = quote_activer(parser->quote, parser->str[i]);
		if (!parser->quote && (!parser->ignore || parser->str[i + 1] == '\0'))
			parser->pos = manage_parsing(&cmds, &i,
				parser->pos, parser->str);
		if (parser->ignore && parser->str[i] != '\\')
			parser->ignore = 0;
	}
	shell->parse_err = parser->quote || parser->ignore ? -1 : parser->pos;
	ft_del(parser->str);
	ft_del(parser);
	return (cmds);
}
