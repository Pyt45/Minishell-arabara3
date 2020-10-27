#include "../../includes/shell.h"

// void	write_to_file(char *s, char *num, int end)
// {
// 	FILE *f;
// 	int fd;

// 	f = fopen("debug.txt", "a");
// 	fd = fileno(f);
// 	ft_putstr_fd(s, fd);
// 	if (num)
// 		ft_putstr_fd(num, fd);
// 	if (end)
// 		ft_putstr_fd("\n-------\n", fd);
// 	fclose(f);
// }

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
		while (!ft_isalnum(config->str[config->c]) && config->str[config->c])
			config->c++;
	}
	else if (dir == 5 || dir == 6)
	{
		if (config->c - config->width > 0 && dir == 5)
			config->c -= config->width;
		else if (dir == 6 && config->len >= config->c + config->width)
			config->c += config->width;
	}
	display_cursor(config);
}

void	get_cursor_pos(t_config *config)
{
	int		i;
	char	buff[20];

	i = 0;
	ft_bzero(buff, sizeof(char) * 20);
	ft_putstr_fd("\e[6n", 2);
	while (buff[0] != '\e')
	{
		read(2, buff, sizeof(buff));
		if (buff[0] != '\e')
		{
			if (config->tmp)
				config->tmp = ft_strjoin(config->tmp, buff);
			else
				config->tmp = ft_strdup(buff);
		}
	}
	while (!ft_isdigit(buff[i]))
		i++;
	config->o_y = ft_atoi(buff + i) - 1;
	while (ft_isdigit(buff[i]))
		i++;
	config->o_x = ft_atoi(buff + i + 1) - 1;
}