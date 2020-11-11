/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 09:42:57 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/11 09:42:58 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	handle_control(t_config *config)
{
	if (config->buff == ALT_V)
		paste_control(config);
	else if (config->buff == ALT_X)
		set_control(config, 2);
	else if (config->buff == ALT_C)
		set_control(config, 1);
	else if (config->buff == ALT_S)
		set_control(config, 0);
}

void	handle_btns(t_config *config)
{
	if (config->buff == HOME_BTN)
	{
		config->c = config->len;
		move_cursor(config, 0);
	}
	else if (config->buff == END_BTN)
	{
		config->c = 0;
		move_cursor(config, 0);
	}
	else if (config->buff == BCK_SP)
		delete_char(config);
}

void	handle_keys(t_config *config)
{
	if (config->buff == ARRW_LEFT)
		move_cursor(config, 1);
	else if (config->buff == ARRW_RIGHT)
		move_cursor(config, 3);
	else if (config->buff == ALT_LEFT)
		move_cursor(config, 2);
	else if (config->buff == ALT_RIGHT)
		move_cursor(config, 4);
	else if (config->buff == ALT_UP)
		move_cursor(config, 5);
	else if (config->buff == ALT_DOWN)
		move_cursor(config, 6);
	else if (config->buff == ARRW_UP)
		display_history(config, 0);
	else if (config->buff == ARRW_DOWN)
		display_history(config, 1);
	handle_btns(config);
	handle_control(config);
}
