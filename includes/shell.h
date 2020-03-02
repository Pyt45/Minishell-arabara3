/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 10:50:30 by zlayine           #+#    #+#             */
/*   Updated: 2020/03/01 19:16:47 by aaqlzim          ###   ########.fr       */
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

typedef struct		s_cmds
{
    char			*cmd;
    char			**args;
    struct s_cmds 	*next;
}					t_cmds;

typedef struct		s_shell
{
    char			**env;
	char			*line;
    int				num_pipe;
    int				spr;
    struct s_cmds   *cmds;
}					t_shell;

char				*get_cmd(char *str);
char				**get_args(char *str);
t_shell				*get_commands(t_shell *shell, char split);
int					ft_access(char *path, int mode);
int					ft_getenv(char *name, char **env);
char				*try_path(char *filename, char *dir);
char				*get_bin_path(char *filename, char **env);
t_shell				*control_command(t_shell *shell);
t_shell				*get_single_command(t_shell *shell);
int					excute_command_by_order(t_shell *shell);
int					exec_commands(t_shell *shell);
int					cd_builtin(t_shell *shell);
int					command_line(t_shell *shell);
int					exit_builtin(t_shell *shell);
char				*get_home_dir(t_shell *shell);
char				*get_old_dir(t_shell *shell);
char				*ft_strcat(char *dest, char *src);
char				*ft_strcpy(char *s1, char *s2);
int					ft_arr_len(char **arr);
char				**ft_arrdup(char **arr);
void				ft_print_env_arr(char **arr);
int					ft_free_arr(char **arr);
int					env_builtin(char **argv, char **env);
void				sig_handle_ctrl_c(int signal);
char				**ft_setenv(char *var, char *path, char **env);
int					pwd_builtin(t_shell *shell);
char				**ft_export_cmd(t_shell *shell, char *value);
int					export_builtin(t_shell *shell);
char				**ft_add_to_arr(char *value, char **arr);
char				**ft_get_arr(char *value, char **arr);
char				**ft_remove_from_arr(int pos, char **env);
char				**ft_unset_cmd(t_shell *shell, char *value);
int					unset_builtin(t_shell *shell);
char				**ft_unset_cmd(t_shell *shell, char *value);
char				**ft_copy_arr_without(int pos, char **arr, char **new_arr, int len);
#endif