/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 10:50:30 by zlayine           #+#    #+#             */
/*   Updated: 2020/02/28 16:55:32 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <dirent.h>
# include <stdio.h>
# include "../libft/libft.h"
# define BUFFER_SIZE 1

typedef struct  s_cmds
{
    char    *cmd;
    char    **args;
    t_cmds  *next;
}               t_cmds;

typedef struct  s_shell
{
    char    **env;
    char    **result;
    t_cmds  cmds;
}               t_shell;

#endif