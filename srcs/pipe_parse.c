/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:12:59 by zlayine           #+#    #+#             */
/*   Updated: 2020/02/28 19:28:17 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

char    *get_cmd(char *str)
{
    int     i;
    char    *cmd;

    i = 0;
    while (!ft_isalpha(*str))
        str++;
    while (ft_isalpha(str[i]))
        i++;
    cmd = malloc(sizeof(char) * (++i));
    ft_strlcpy(cmd, str, i);
    return (cmd);
}

char    **get_args(char *str)
{
    int     i;

    i = 0;
    while (!ft_isalpha(*str))
        str++;
    while (ft_isalpha(*str))
        str++;
    return (ft_split(str, ' '));
}

t_cmds  *get_commands(char *str)
{
    t_shell shell;
    t_cmds  *cmds;
    t_cmds  *head;
    char **tmp = ft_split(str, '|');

    if (*tmp)
    {
        cmds = malloc(sizeof(t_cmds));
        head = cmds;
    }
    while (*tmp)
    {
        cmds->cmd = get_cmd(*tmp);
        printf("COMMAND: %s|\n", cmds->cmd);
        cmds->args = get_args(*tmp);
        printf("ARG1: %s|\n", cmds->args[0]);
        tmp++;
        if (*tmp)
        {
            cmds->next = malloc(sizeof(t_cmds));
            cmds = cmds->next;
        }
    }
   return (head);
}