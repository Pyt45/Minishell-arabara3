/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:10:44 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/18 10:42:08 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		check_parsing(t_shell *shell)
{
	if (shell->parse_err == -1)
	{
		print_error("syntax error", 0, 0);
		shell->ret = 258;
		return (0);
	}
	return (1);
}

t_shell	*parse_commands(t_shell *shell)
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
		if (parser->str[i] == '\\' || parser->ignore)
			parser->ignore = parser->ignore ? 0 : 1;
		else if (!parser->ignore)
		{
			if (is_quote(parser->str[i], 0) && !parser->ignore)
				parser->quote = quote_activer(parser->quote, parser->str[i]);
			if (!parser->quote && (parser->str[i] == ';' || parser->str[i + 1] == '\0'))
				parser->pos = create_cmd_line(&cmds, parser->str, parser->pos, i);
		}
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
		if (parser->str[i] == '\\' || parser->ignore)
			parser->ignore = parser->ignore ? 0 : 1;
		else if (!parser->ignore)
		{
			if (is_quote(parser->str[i], 0))
			{
				parser->quote = quote_activer(parser->quote, parser->str[i]);
			}
			if (!parser->quote)
				parser->pos = manage_parsing(&cmds, &i, parser->pos, parser->str);
		}
	}
	shell->parse_err = parser->quote || parser->ignore ? -1 : parser->pos;
	ft_del(parser->str);
	ft_del(parser);
	return (cmds);
}
