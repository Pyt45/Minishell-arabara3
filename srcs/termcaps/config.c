#include "../../includes/shell.h"

void	display_tmp_data(t_config *config)
{
	int i;

	i = 0;
	if (config->tmp)
	{
		while (config->tmp[i])
		{
			config->buff = config->tmp[i];
			if (ft_isprint(config->buff))
				print_char(config);
			else if (config->buff == ENTER_BTN)
				break;
			i++;
		}
	}
	if (config->buff != ENTER_BTN)
	{
		config->tmp = NULL;
		config->buff = 0;
	}
	else
		config->tmp = config->tmp + i + 1;
}

void	init_prompt(t_config *config, int err)
{
	if (config->history)
		config = add_history(config);
	else
		init_config_data(config);
	ft_bzero(config->str, 512 * sizeof(char));
	config->c = 0;
	config->len = 0;
	config->buff = 0;
	config->term.c_lflag &= ~(ICANON | ECHO);
	config->cursor = tgetstr("cm", NULL);
	if (tcsetattr(0, 0, &config->term) == -1)
		printf("this is an error");
	if (err != 2)
		ft_putstr_fd("\033[1;32mminishell~>\033[0m", 1);
	get_cursor_pos(config);
	config->x = config->o_x;
	config->y = config->o_y;
	display_cursor(config);
	display_tmp_data(config);
}

void	init_config_data(t_config *config)
{
	char	*term;
	int		ret;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	// if (ret < 1)
	// 	printf("this is an error term 1");
	tcgetattr(0, &config->term);
	// if (tcgetattr(0, &config->term) == -1)
	// 	printf("this is an error term 2");
	config->str = malloc(sizeof(char) * 512);
	config->history = malloc(sizeof(t_history));
	config->history->prev = NULL;
	config->history->next = NULL;
	config->width = tgetnum("co");
	config->height = tgetnum("li");
	config->tmp = NULL;
	config->o_x = 0;
	config->o_y = 0;
}

void	end_terminal(t_config *config)
{
	config->term.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, 0, &config->term);
	// if (tcsetattr(0, 0, &config->term) == -1)
	// 	printf("this is an error endterm");
}
