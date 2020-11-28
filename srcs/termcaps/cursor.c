/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:01:52 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/20 10:08:34 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	display_cursor(t_config *config)
{
	if (config->x == 0 && config->y == 0)
	{
		config->o_y++;
		tputs(tgetstr("sr", 0), 0, ft_putchars);
		print_line_up(config);
	}
	config->x = (config->o_x + config->c) % config->width;
	config->y = ((config->o_x + config->c) / config->width) + config->o_y;
	tputs(tgoto(config->cursor, config->x, config->y), 0, ft_putchars);
	if (config->o_y + ((config->o_x + config->c) /
		config->width) >= config->height)
	{
		if (config->x % config->width == 0)
			tputs(tgetstr("sf", 0), 0, ft_putchars);
		config->o_y--;
		print_line_down(config);
	}
}

void	move_cursor(t_config *config, int dir)
{
	if (dir == 1 && config->c + config->x > config->o_x)
		config->c--;
	else if (dir == 3 && config->c < config->len)
		config->c++;
	else if (dir == 2 && config->c)
	{
		while (!ft_isalnum(config->str[config->c - 1]))
			config->c--;
		while (ft_isalnum(config->str[config->c - 1]))
			config->c--;
	}
	else if (dir == 4)
	{
		while (ft_isalnum(config->str[config->c]))
			config->c++;
		while (!ft_isalnum(config->str[config->c]) &&
			config->str[config->c])
			config->c++;
	}
	else if (dir == 5 && config->c - config->width > 0)
		config->c -= config->width;
	else if (dir == 6 && config->len >= config->c + config->width)
		config->c += config->width;
	display_cursor(config);
}

void	get_cursor_pos(t_config *config)
{
	int		i;
	char	*buff;

	i = 0;
	buff = get_cursor_buff(config, 1);
	while (!ft_isdigit(buff[i]))
		i++;
	config->o_y = ft_atoi(buff + i) - 1;
	while (ft_isdigit(buff[i]))
		i++;
	config->o_x = ft_atoi(buff + i + 1);
	ft_del(buff);
	// write_to_file("OX ", ft_itoa(config->o_x), 1);
	// write_to_file("OY ", ft_itoa(config->o_y), 1);
}

void	reinit_cursor(t_config *config, int new_x, int new_y, int reinit)
{
	config->o_x = new_x;
	config->o_y = new_y;
	config->x = config->o_x;
	config->y = config->o_y - 1;
	if (reinit)
	{
		config->c = 0;
		config->len = 0;
		ft_bzero(config->str, 512 * sizeof(char));
	}
	move_cursor(config, 3);
}

void	validate_cursor(t_config *config, t_shell *shell)
{
	int		new_y;
	int		new_x;
	int		i;
	char	*buff;

	i = 0;
	buff = get_cursor_buff(config, 0);
	while (!ft_isdigit(buff[i]))
		i++;
	new_y = ft_atoi(buff + i) - 1;
	while (ft_isdigit(buff[i]))
		i++;
	new_x = ft_atoi(buff + i + 1);
	ft_del(buff);
	// if (new_y > config->y)
	// 	write_to_file("CASE 1 ", ft_itoa(new_y > config->y), 1);
	// if (new_y == config->y && new_x == config->o_x && !shell->mutli && (config->len < config->width - 1))
	// 	write_to_file("CASE 2 ", ft_itoa((new_y == config->y && new_x == config->o_x && !shell->mutli && (config->len < config->width - 1))), 1);
	// if (config->o_x == new_x && config->len == config->width)
	// 	write_to_file("CASE 3 ", ft_itoa((config->o_x == new_x && config->len == config->width)), 1);
	if ((new_y > config->y || (new_y == config->y && new_x ==
		config->o_x && (config->len < config->width - 1)) || (config->o_x ==
		new_x && config->len == config->width)))
	{
		shell->ret = 1;
		reinit_cursor(config, new_x, new_y, 1);
	}
}
