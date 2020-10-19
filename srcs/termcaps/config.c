#include "../../includes/shell.h"

void	re_init_shell(t_config *config)
{
	config = add_history(config);
	ft_bzero(config->str, config->len * sizeof(char));
	config->c = 0;
	config->buff = 0;
	config->len = 0;
	ft_putstr_fd("\033[1;32mminishell~>\033[0m", 1);
	get_cursor_pos(config);
	display_cursor(config);
}

void	init_config_data(t_config *config)
{
	config->len = 0;
	config->str = malloc(sizeof(char) * 512);
	ft_bzero(config->str, sizeof(char) * 512);
	config->buff = 0;
	config->history = malloc(sizeof(t_history));
	config->history->prev = NULL;
	config->history->next = NULL;
	config->width = tgetnum("co");
	config->height = tgetnum("li");
	config->o_x = 0;
	config->o_y = 0;
	get_cursor_pos(config);
	config->x = config->o_x;
	config->y = config->o_y;
	move_cursor(config, 0);
}

void	init_config(t_config *config)
{
	char	*term;
	int		ret;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	if (ret < 1)
		printf("this is an error");
	if (tcgetattr(0, &config->term) == -1)
		printf("this is an error");
	config->term.c_lflag &= ~(ICANON | ECHO);
	config->cursor = tgetstr("cm", NULL);
	if (tcsetattr(0, 0, &config->term) == -1)
		printf("this is an error");
}

void	end_terminal(t_config *config)
{
	config->term.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(0, 0, &config->term) == -1)
		printf("this is an error");
}
