
#ifndef SHELL_H
# define SHELL_H

# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <dirent.h>
# include <stdio.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "../libft/libft.h"

typedef struct		s_exec
{
	int		backup[3];
	int		fdin;
	int		fdout;
	int		j;
	int		*fds;
}					t_exec;

typedef struct		s_cmds
{
	int				start;
	int				end;
	int				p;
	int				append;
	char			*cmd;
	char			**args;
	int				ret;
	int				skip;
	struct s_cmds	*prev;
	struct s_cmds	*next;
}					t_cmds;

typedef struct		s_control
{
	char	*str;
	int		start;
	int		len;
	int		cut;
}					t_control;

typedef struct		s_config
{
	struct termios		term;
	char				*str;
	int					x;
	int					y;
	int					o_x;
	int					o_y;
	int					width;
	int					height;
	char				*cursor;
	long				buff;
	int					c;
	int					len;
	int					cmd;
	char				*tmp;
	struct s_history	*history;
	struct s_control	control;
}					t_config;

typedef struct		s_shell
{
	char			**env;
	char			*line;
	int				ret;
	int				parse_err;
	struct s_cmds	*cmds;
	struct s_config	config;
	struct s_exec	exec;
	int				num_pipe;
	int				num_sp;
}					t_shell;

typedef struct		s_history
{
	char				*data;
	struct s_history	*prev;
	struct s_history	*next;
}					t_history;

# define ARRW_LEFT 4479771
# define ARRW_RIGHT 4414235
# define ALT_LEFT 1146821403
# define ALT_RIGHT 1130044187
# define HOME_BTN 4610843
# define END_BTN 4741915
# define ALT_V 10127586
# define ALT_X 8948194
# define ALT_C 42947
# define ALT_S 40899
# define ALT_UP 1096489755
# define ALT_DOWN 1113266971
# define ARRW_UP 4283163
# define ARRW_DOWN 4348699
# define BCK_SP 127
# define ENTER_BTN 10
# define SIG_C 3
# define SIG_D 4
# define SIG_SL 28

char				*get_cmd(char *str, int n);
char				**get_args(char *str, int n);
t_shell				*parse_commands(t_shell *shell);
t_shell				*get_comma_commands(t_shell *shell);
int					ft_access(char *path, int mode);
int					ft_getenv(char *name, char **env);
char				*try_path(char *filename, char *dir);
char				*get_bin_path(char *filename, char **env);
t_shell				*control_command(t_shell *shell);
t_shell				*get_single_command(t_shell *shell);
t_cmds				*excute_command_by_order(t_shell *shell, t_cmds *cmds);
pid_t				run_child(t_shell *shell, t_cmds *cmds);
int					*pipe_fds(int num_pipe, int *fds);
int					*create_fds(t_cmds *cmds, int j, int *fds);
int					open_input(char *args);
int					get_num_pipes(t_cmds *cmds);
int					is_builtin(char *cmd);
void				close_pipes(int *fds, int num_pipe);
int					get_status_number(int status);
int					wait_child(t_shell *shell, pid_t pid, int st);
int					redirect_forward(t_cmds *tmp, t_cmds *cmd);
int					redirect_backward(t_cmds *tmp);
void				do_redirect(t_cmds *cmd, int *fd);
void				exec_io_redi(t_shell *shell, t_cmds *cmd);
int					open_output(t_cmds *cmd, int append);
int					exec_commands(t_shell *shell, t_cmds *cmds);
int					run_commands(t_shell *shell);
int					cd_builtin(t_shell *shell, t_cmds *cmds);
int					exit_builtin(t_shell *shell, t_cmds *cmds);
char				*get_home_dir(t_shell *shell);
char				*get_old_dir(t_shell *shell);
char				*ft_strcat(char *dest, char *src);
char				*ft_strcpy(char *s1, char *s2);
int					ft_arr_len(char **arr);
char				**ft_arrdup(char **arr);
void				ft_print_env_arr(char **arr);
int					ft_free_arr(char **arr);
int					env_builtin(t_cmds *cmd, char **env);
void				sig_handle_ctrl_c();
char				**ft_setenv(char *var, char *path, char **env);
int					pwd_builtin();
int					ft_export_cmd(t_shell *shell, char *value);
int					export_builtin(t_shell *shell, t_cmds *cmds);
char				**ft_add_to_arr(char *value, char **arr);
char				**ft_get_arr(char *value, char **arr);
char				**ft_remove_from_arr(int pos, char **env);
char				**ft_unset_cmd(t_shell *shell, char *value);
int					unset_builtin(t_shell *shell, t_cmds *cmds);
char				**ft_unset_cmd(t_shell *shell, char *value);
char				**ft_copy_arr_without(int pos,
	char **arr, char **new_arr, int len);
int					echo_builtin(t_cmds *cmd);
void				print_line_up(t_config *config);
void				print_line_down(t_config *config);
void				display_cursor(t_config *config);
void				move_cursor(t_config *config, int dir);
void				display_content(t_config *config);
void				delete_char(t_config *config);
void				print_char(t_config *config);
void				get_cursor_pos(t_config *config);
t_history			*new_history(t_history *prev);
void				free_next_history(t_history **history);
t_config			*add_history(t_config *config);
void				display_history(t_config *config, int dir);
void				re_init_shell(t_config *config);
void				paste_control(t_config *config);
void				cut_control(t_config *config);
void				copy_control(t_config *config);
void				set_control(t_config *config, int a);
void				handle_control(t_config *config);
void				handle_btns(t_config *config);
void				handle_keys(t_config *config);
void				init_config_data(t_config *config);
void				end_terminal(t_config *config);
int					ft_putchars(int c);
void				newline_config(t_config *config, int x);
int					print_error(char *cmd, int err, int iscmd);
char				*clear_quotes(char *str);
int					is_quote(char c, int type);
char				**ft_split_quote(char const *s, char c);
char				*replace_string(char *str, t_shell *shell);
void				init_prompt(t_config *config, int err);
void				reinit_cursor(t_config *config, int new_x, int new_y);
void				validate_cursor(t_config *config, t_shell *shell);
char				*parse_special_chars(char *str);
void				manage_redirections(t_cmds **cmds, int *i, char *tmp);
t_cmds				*init_cmds(t_cmds   *prev);
char				*parse_env_var(char *str, t_shell *shell);
void				quotes_checker(int *quote, int c);
int					var_checker_pass(char c);
int					quote_activer(int quote, char c);
void				free_shell(t_shell *shell);
void				init_shell(t_shell *shell);
void				free_config(t_config *config);
int					valid_arg_name(char *val);
void				exit_error(char *str, int status, t_shell *shell);
char				*get_cursor_buff(t_config *config, int active);

// debuging
void				write_to_file(char *s, char *num, int end);
void				debug_cmd(t_cmds *cmds, int i, int pos, char c);
void				erase_file_debug();

#endif
