/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:12:59 by zlayine           #+#    #+#             */
/*   Updated: 2020/02/28 19:07:33 by zlayine          ###   ########.fr       */
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

t_cmds  *get_commands(t_shell *shell, char split)
{
    t_cmds  *cmds;

    char **tmp = ft_split(shell->line, split);
    if (*tmp)
    {
        cmds = malloc(sizeof(t_cmds));
        shell->cmds = cmds;
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
    return (shell->cmds);
}

// --------------- TEST -------------------

// int main()
// {
//     t_shell shell;

//     shell.line = ft_strdup("env | grep PATH | cat -e");
//     shell.cmds = get_commands(&shell, '|');
//     shell.line = ft_strdup("echo PATH;find .;env");
//     shell.cmds = get_commands(&shell, ';');
// }
