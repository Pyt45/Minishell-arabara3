#include "../../includes/shell.h"

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
}

void	init_config_data(t_config *config)
{
	char	*term;
	int		ret;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	if (ret < 1)
		printf("this is an error");
	if (tcgetattr(0, &config->term) == -1)
		printf("this is an error");
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
	if (tcsetattr(0, 0, &config->term) == -1)
		printf("this is an error");
}
