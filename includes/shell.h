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
    struct s_cmds   *cmds;
}					t_shell;

char    *get_cmd(char *str);
char    **get_args(char *str);
t_shell	*get_commands(t_shell *shell, char split);
int     ft_access(char *path, int mode);
int     ft_getenv(char *name, char **env);
char	*try_path(char *filename, char *dir);
char    *get_bin_path(char *filename, char **env);
t_shell     *control_command(t_shell *shell);
t_shell     *get_single_command(t_shell *shell);
char    *ft_strcat(char *dest, char *src);
char    *ft_strcpy(char *s1, char *s2);
int     ft_arr_len(char **arr);
char    **ft_arrdup(char **arr);
void    ft_print_env_arr(char **arr);
int		ft_free_arr(char **arr);

#endif