/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 10:10:18 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/17 18:02:38 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		excute_cmd_help(t_shell *shell, t_cmds *cmds, pid_t pid)
{
	int		status;
	float	shift;

	status = 0;
	close_pipes(shell->exec.fds, shell->num_pipe);
	status = wait_child(shell, pid);
	if (status == 2 || status == 3)
		cmds->ret = status + 128;
	else
	{
		shift = (status) / (ft_pow(2, 8));
        cmds->ret = (int)shift & 255;
	}
	// if (status == 2 || status == 3)
	// 	cmds->ret = (status & 0177) + 128;
	// else
	// 	cmds->ret = (status >> 8) & 0x000000ff;
	if (shell->num_pipe)
	{
		ft_del(shell->exec.fds);
		ft_del(shell->exec.pids);
	}
}

static t_cmds	*excute_loop_append(t_cmds *cmds)
{
	while (cmds && cmds->append > 0)
	{
		if (!cmds->next)
			break ;
		cmds = cmds->next;
	}
	return (cmds);
}

void			save_fds(int *fds)
{
	fds[0] = dup(0);
	fds[1] = dup(1);
	fds[2] = dup(2);
}

t_cmds			*excute_command_by_order(t_shell *shell, t_cmds *cmds)
{
	pid_t	pid;

	if ((cmds->next && !cmds->end) || !is_builtin(cmds->cmd))
	{
		shell->exec.fds = pipe_fds(shell->num_pipe, shell->exec.fds);
		save_fds(shell->exec.backup);
		while (cmds)
		{
			if (!cmds->skip)
			{
				pid = run_child(shell, cmds);
				cmds = excute_loop_append(cmds);
				shell->exec.j += 2;
			}
			if (cmds->end)
				break ;
			else
				cmds = cmds->next;
		}
		restore_fds(shell->exec.backup);
		excute_cmd_help(shell, cmds, pid);
	}
	else if (cmds->cmd)
		cmds->ret = exec_commands(shell, cmds);
	return (cmds);
}

t_cmds			*parse_command(t_shell *shell, t_cmds *cmds)
{
	t_parser	*parser;
	int			i;

	parser = init_parser(shell, cmds->line, 1);
	i = -1;
	while (parser->str[++i] && parser->pos != -1)
	{
		if (parser->str[i] == '\\' || parser->ignore)
			parser->ignore = parser->ignore ? 0 : 1;
		else if (!parser->ignore)
		{
			if (is_quote(parser->str[i], 0) && !parser->ignore)
				parser->quote = quote_activer(parser->quote, parser->str[i]);
			if (!parser->quote)
				parser->pos = manage_parsing(&cmds, &i,
					parser->pos, parser->str);
		}
	}
	shell->parse_err = parser->quote || parser->ignore ? -1 : parser->pos;
	ft_del(parser->str);
	ft_del(parser);
	return (shell);
}

void			run_commands(t_shell *shell)
{
	t_cmds	*cmds;

	shell->num_pipe = 0;
	shell->parse_err = 0;
	shell = parse_commands(shell);
	// if (shell->parse_err == -1)
	// {
	// 	print_error("syntax error", 0, 0);
	// 	shell->ret = 258;
	// }
	// else
	// {
	// 	cmds = shell->cmds;
	// 	while (cmds)
	// 	{
	// 		cmds = parse_command(shell, cmds);
	// 		signal(SIGQUIT, sig_handle);
	// 		shell->exec.j = 0;
	// 		shell->num_pipe = get_num_pipes(cmds);
	// 		if (shell->num_pipe)
	// 			shell->exec.pids = malloc(sizeof(int) * (shell->num_pipe + 1));
	// 		cmds = excute_command_by_order(shell, cmds);
	// 		shell->ret = cmds->ret;
	// 		cmds = cmds->next;
	// 		int i = ft_getenv("OLDPWD", shell->env);
	// 		printf("mine: %s\n", shell->env[i]);
	// 	}
	// }
}
