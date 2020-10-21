/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 13:25:01 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/21 10:18:30 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		ft_putchars(int c)
{
	write(0, &c, 1);
	return (0);
}

void	display_content(t_config *config)
{
	config->str[config->len] = 0;
	ft_putchars((int)config->str[config->len]);
	display_cursor(config);
}

void	delete_char(t_config *config)
{
	int	i;

	if (config->c < config->len && config->c)
	{
		i = config->c - 1;
		config->c--;
		display_cursor(config);
		tputs(tgetstr("cd", NULL), 0, ft_putchars);
		while (i < config->len)
		{
			config->str[i] = config->str[i + 1];
			ft_putchars((int)config->str[i]);
			i++;
		}
		config->len--;
		display_content(config);
	}
	else if (config->c)
	{
		config->c--;
		config->len--;
		display_content(config);
		tputs(tgetstr("cd", NULL), 0, ft_putchars);
	}
}

void	print_char(t_config *config)
{
	char	tmp;
	int		i;

	if (config->c < config->len)
	{
		i = config->c;
		tputs(tgetstr("cd", NULL), 0, ft_putchars);
		while (i <= config->len)
		{
			tmp = config->str[i];
			config->str[i] = config->buff;
			config->buff = tmp;
			ft_putchars((int)config->str[i++]);
		}
		config->c++;
		config->len++;
	}
	else
	{
		config->str[config->c] = config->buff;
		ft_putchars((int)config->buff);
		config->c++;
		config->len = config->c;
	}
	display_content(config);
}
