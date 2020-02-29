
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
    return (ft_split(str, ' '));
}

t_shell  *get_commands(t_shell *shell, char split)
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
        //printf("COMMAND: %s|\n", cmds->cmd);
        cmds->args = get_args(*tmp);
        //printf("ARG0: %s|\n", cmds->args[0]);
        //printf("ARG1: %s|\n", cmds->args[1]);
        tmp++;
        if (*tmp)
        {
            cmds->next = malloc(sizeof(t_cmds));
            cmds = cmds->next;
        }
    }
    return (shell);
}

t_shell     *get_single_command(t_shell *shell)
{
    shell->cmds = malloc(sizeof(t_cmds));
    shell->cmds->cmd = get_cmd(shell->line);
    shell->cmds->args = get_args(shell->line);
    shell->cmds->next = NULL;
}

t_shell     *control_command(t_shell *shell)
{
    if (ft_strchr(shell->line, '|'))
        return (get_commands(shell, '|'));
    else if (ft_strchr(shell->line, ';'))
        return (get_commands(shell, ';'));
    else
        return (get_single_command(shell));
}

// --------------- TEST -------------------
/* 
int main()
{
    t_shell shell;

    shell.line = ft_strdup("env | grep PATH | cat -e");
    shell.cmds = get_commands(&shell, '|');
    shell.line = ft_strdup("echo PATH;find .;env");
    shell.cmds = get_commands(&shell, ';');
}
 */