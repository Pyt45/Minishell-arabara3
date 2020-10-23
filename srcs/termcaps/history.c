#include "../../includes/shell.h"

t_history	*new_history(t_history *prev)
{
	t_history *history;

	history = malloc(sizeof(t_history));
	history->data = "";
	history->prev = prev;
	history->next = NULL;
	return (history);
}

void		free_next_history(t_history **history)
{
	t_history	*curr;
	t_history	*tmp;

	curr = (*history)->next;
	while (curr)
	{
		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
	(*history)->next = NULL;
}

t_config	*add_history(t_config *config)
{
	if (config->history->next){
		while(config->history->next)
			config->history = config->history->next;
	}
	config->history->data = ft_strdup(config->str);
	if (config->history->next)
		free_next_history(&config->history);
	config->history->next = new_history(config->history);
	config->history = config->history->next;
	return (config);
}

void		display_history(t_config *config, int dir)
{
	int	exist;

	exist = 0;
	if (dir && config->history->next)
	{
		config->history = config->history->next;
		exist = 1;
	}
	else if (dir == 0 && config->history->prev)
	{
		config->history = config->history->prev;
		exist = 1;
	}
	if (exist)
	{
		tputs(tgoto(config->cursor, config->o_x, config->o_y), 0, ft_putchars);
		tputs(tgetstr("cd", NULL), 0, ft_putchars);
		ft_strcpy(config->str, config->history->data);
		config->len = ft_strlen(config->str);
		config->c = config->len;
		ft_putstr_fd(config->str, 0);
		display_cursor(config);
	}
}
