/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 10:50:30 by zlayine           #+#    #+#             */
/*   Updated: 2020/03/04 19:38:52 by zlayine          ###   ########.fr       */
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

typedef struct		s_b_cmds
{
    char			*cmd;
    int				num_pipe;
    struct s_m_cmds *m_cmds;
    struct s_b_cmds *next;
}					t_b_cmds;

typedef struct      s_m_cmds
{
    char            *cmd;
    struct s_s_cmds *s_cmds;
    struct s_m_cmds *next;
}                   t_m_cmds;

typedef struct      s_s_cmds
{
    char            *cmd;
    char            **args;
	int				append;
    struct s_s_cmds *next;
}                   t_s_cmds;

// typedef struct		s_cmds
// {
//     char			*cmd;
//     char			**args;
//     struct s_cmds 	*next;
// }					t_cmds;

typedef struct		s_shell
{
    char			**env;
	char			*line;
    int             ret;
    struct s_b_cmds	*cmds;
}					t_shell;

# define IS_S_QUOTE(x) (x == '\'')
# define IS_QUOTE(x) (x == '"' || x == '\'')

char                *get_cmd(char *str, int n);
char                **get_args(char *str, int n);
t_s_cmds            *add_command(t_s_cmds *cmds, char *cmd, int i);
t_s_cmds            *get_commands(t_m_cmds *m_cmd, char *cmd);
t_m_cmds            *get_pipe_commands(t_b_cmds *b_cmd);
t_shell             *get_comma_commands(t_shell *shell);
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
// int					echo_builtin(t_cmds *cmd, char **env);

#endif