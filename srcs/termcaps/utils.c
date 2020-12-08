/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 13:25:01 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/14 09:46:50 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		ft_putchars(int c)
{
	write(0, &c, 1);
	return (0);
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

char	*get_cursor_buff(t_config *config, int active)
{
	char	*buff;
	char	*tmp;

	buff = malloc(sizeof(char) * 20);
	ft_bzero(buff, sizeof(char) * 20);
	ft_putstr_fd("\e[6n", 2);
	while (buff[0] != '\e')
	{
		read(2, buff, sizeof(buff));
		if (buff[0] != '\e' && !active)
			ft_putstr_fd("\e[6n", 2);
		else if (buff[0] != '\e' && active)
		{
			if (config->tmp)
			{
				tmp = config->tmp;
				config->tmp = ft_strjoin(config->tmp, buff);
				ft_del(tmp);
			}
			else
				config->tmp = ft_strdup(buff);
		}
	}
	return (buff);
}
