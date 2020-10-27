/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 10:29:31 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/27 10:39:15 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	free_shell(t_shell *shell)
{
	int i;
	int debug = 0;
	t_cmds *tmp;
	// free all the data and re-init
	shell->line = NULL;
	if (debug)
		write_to_file("FREEING", "", 1);
	if (shell->cmds && shell->cmds->cmd)
	{
		if (debug)
			write_to_file(" 1 ", "", 0);
		while(shell->cmds)
		{
			if (debug)
				write_to_file(" 1x ", "", 0);
			i = 0;
			if (shell->cmds->args)
			{
				if (debug)
					write_to_file(" 2 ", "", 0);
				while (shell->cmds->args[i])
				{
					ft_del(shell->cmds->args[i]);
					if (debug)
						write_to_file(" 2x ", "", 0);
					i++;
				}
				ft_del(shell->cmds->args);
				if (debug)
					write_to_file(" 3 ", "", 0);
			}
			ft_del(shell->cmds->cmd);
			if (debug)
				write_to_file(" 4 ", "", 0);
			tmp = shell->cmds->next;
			ft_del(shell->cmds);
			if (debug)
				write_to_file(" 5 ", "", 0);
			shell->cmds = tmp;
		}
	}
	if (debug)
		write_to_file(" END ", "", 1);
	shell->cmds = NULL;
}

void	free_config(t_config *config)
{
	t_history	*history;
	t_history	*tmp;

	history = config->history;
	ft_del(config->str);
	ft_del(config->tmp); // this is not fully freed
	while (history->prev)
		history = history->prev;
	while (history->next)
	{
		ft_del(history->data);
		tmp = history->next;
		free(history);
		history = tmp;
	}
	config->history = NULL;
}

void		init_shell(t_shell *shell)
{
	shell->line = NULL;
	shell->parse_err = 0;
	shell->ret = 0;
	shell->cmds = NULL;
}
