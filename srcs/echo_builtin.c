
#include "../includes/shell.h"

static void	ft_putnstr(char *str, int n)
{
	int		i;

	i = -1;
	if (n < 0)
	{
		while (str[++i] && i < (int)ft_strlen(str) + n)
			ft_putchar_fd(str[i], 1);
	}
	else
	{
		while (str[++i] && i < n)
			ft_putchar_fd(str[i], 1);
	}
}

# define IS_QUOTE(x) (x == '"' || x == '\'')

static void echo_print(char **str, int pos)
{
    int     s_with;
    int     e_with;
    int     len;

    s_with = IS_QUOTE(str[pos][0]);
	len = (int)ft_strlen(str[pos]);
	e_with = IS_QUOTE(str[pos][len - 1]);
	if (e_with && s_with)
		ft_putnstr(str[pos] + 1, -1);
	else if (e_with)
		ft_putnstr(str[pos], -1);
	else if (s_with)
		ft_putstr_fd(str[pos] + 1, 1);
	else
		ft_putstr_fd(str[pos], 1);
	if (str[pos + 1])
		ft_putchar_fd(' ', 1);
}

int     echo_command(t_cmds *cmd)
{
    int     i;
    int     n_flag;

    n_flag = 0;
    if (!cmd->args[0])
    {
        write(1, "\n", 1);
        return (1);
    }
    else if (cmd->args[0][0] == '-' && cmd->args[0][1] == 'n' && cmd->args[0][2] == '\0')
        n_flag = 1;
    i = -1;
    if (n_flag)
        ++i;
    while (cmd->args[++i])
    {
        echo_print(cmd->args, i);
        if (!cmd->args[i + 1] && !n_flag)
            ft_putchar_fd('\n', 1);
    }
    return (1);
}

// ---------------------- TEST ----------------

int main(int argc, char **argv, char **envp)
{
    t_cmds cmd;
    cmd.cmd = "echo";
    cmd.args[0] = "oh $PATH\"";
    cmd.args[1] = 0;
    echo_command(&cmd);
}