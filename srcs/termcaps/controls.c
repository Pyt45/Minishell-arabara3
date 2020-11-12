/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 09:42:49 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/11 09:42:50 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	paste_control(t_config *cfg)
{
	int		i;
	int		len;

	len = cfg->control.len;
	ft_memmove(cfg->str + cfg->c + len, cfg->str + cfg->c, len + 1);
	ft_memcpy(cfg->str + cfg->c, cfg->control.str, len);
	cfg->len += len;
	i = cfg->c - 1;
	tputs(tgetstr("cd", NULL), 0, ft_putchars);
	while (++i < cfg->len)
		ft_putchars((int)cfg->str[i]);
	cfg->c += len;
	display_cursor(cfg);
}

void	cut_control(t_config *cfg)
{
	cfg->control.cut = 1;
	cfg->control.len = cfg->c - cfg->control.start;
	if (cfg->control.start > cfg->c)
	{
		cfg->control.start = cfg->c;
		cfg->control.len = -cfg->control.len;
	}
	cfg->control.str = malloc(sizeof(char) * (cfg->control.len + 1));
	ft_strlcpy(cfg->control.str, cfg->str +
		cfg->control.start, cfg->control.len + 1);
	while (cfg->c > cfg->control.start)
		delete_char(cfg);
}

void	copy_control(t_config *cfg)
{
	cfg->control.cut = 0;
	cfg->control.len = cfg->c - cfg->control.start;
	if (cfg->control.start > cfg->c)
	{
		cfg->control.start = cfg->c;
		cfg->control.len = -cfg->control.len;
	}
	cfg->control.str = malloc(sizeof(char) * (cfg->control.len + 1));
	ft_strlcpy(cfg->control.str, cfg->str +
		cfg->control.start, cfg->control.len + 1);
}

void	set_control(t_config *cfg, int a)
{
	if (a == 0)
	{
		cfg->control.start = cfg->c;
		cfg->control.str = "";
		cfg->control.len = 0;
		cfg->control.cut = 0;
	}
	else if (a == 1)
		copy_control(cfg);
	else if (a == 2)
		cut_control(cfg);
}
