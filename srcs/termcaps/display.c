/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 09:42:54 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/11 09:42:55 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	print_line_up(t_config *config)
{
	int i;
	int c;

	config->y = ((config->o_x + config->c) / config->width) + config->o_y;
	if (config->o_y == 0)
	{
		i = 0;
		c = 0;
		ft_putstr_fd("minishell~>", 1);
		tputs(tgoto(config->cursor, 12, config->y), 0, ft_putchars);
	}
	else
	{
		i = 0;
		c = config->c - config->width + 1;
		tputs(tgoto(config->cursor, 0, config->y), 0, ft_putchars);
	}
	while (i < config->width && config->str[i + c])
	{
		ft_putchars(config->str[i + c]);
		i++;
	}
}

void	print_line_down(t_config *config)
{
	int i;
	int c;

	config->y = ((config->o_x + config->c) / config->width) + config->o_y;
	i = 0;
	c = config->c;
	tputs(tgoto(config->cursor, 0, config->y), 0, ft_putchars);
	while (i < config->width && config->str[i + c])
	{
		ft_putchars(config->str[i + c]);
		i++;
	}
	display_cursor(config);
}

void	newline_config(t_config *config, int x)
{
	if (config->y == config->height - 1)
	{
		tputs(tgetstr("sf", 0), 0, ft_putchars);
		config->o_y--;
	}
	else
	{
		ft_putstr_fd("\n", 0);
		config->y++;
	}
	tputs(tgoto(config->cursor, x, config->y), 0, ft_putchars);
}
