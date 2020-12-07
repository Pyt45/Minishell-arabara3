/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 10:29:31 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/07 12:35:08 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	free_shell(t_shell *shell)
{
	int		i;
	t_cmds	*tmp;

	shell->line = NULL;
	if (shell->cmds && shell->cmds->line)
	{
		while (shell->cmds)
		{
			i = -1;
			ft_del(shell->cmds->line);
			while (shell->cmds->args && shell->cmds->args[++i])
				ft_del(shell->cmds->args[i]);
			ft_del(shell->cmds->args);
			ft_del(shell->cmds->cmd);
			tmp = shell->cmds->next;
			ft_del(shell->cmds);
			shell->cmds = tmp;
		}
	}
	else if (shell->cmds)
		ft_del(shell->cmds);
	shell->cmds = NULL;
}

void	free_config(t_config *config)
{
	t_history	*history;
	t_history	*tmp;

	history = config->history;
	ft_del(config->str);
	ft_del(config->tmp);
	while (history->prev)
		history = history->prev;
	while (history->next)
	{
		ft_del(history->data);
		tmp = history->next;
		free(history);
		history = tmp;
	}
	free(history);
	config->history = NULL;
}

int		ft_free_arr(char **arr)
{
	int		i;

	i = 0;
	if (!arr)
		return (1);
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	(arr) ? (free(arr)) : 0;
	return (1);
}

void	init_shell(t_shell *shell)
{
	shell->line = NULL;
	shell->parse_err = 0;
	shell->ret = 0;
	shell->cmds = NULL;
}

long	ft_check_long(long n, int sign)
{
	if (n < 0 && sign > 0)
		return (-1);
	if (n > 0 && sign < 0)
		return (0);
	return (n);
}