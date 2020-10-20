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
}

void	newline_config(t_config *config)
{
	ft_putstr_fd("\n", 1);
	if (config->y == config->height - 1)
	{
		tputs(tgetstr("sf", 0), 0, ft_putchars);
		config->o_y--;
	}
	else
		config->y++;
	tputs(tgoto(config->cursor, 0, config->y), 0, ft_putchars);
	end_terminal(config);
}
