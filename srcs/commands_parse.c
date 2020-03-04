
#include "../includes/shell.h"

char    *get_cmd(char *str, int n)
{
    int     i;
    char    *cmd;

    i = 0;
    while (!ft_isalpha(*str))
        str++;
    if (n == 0)
        n = ft_strlen(str);
    while (ft_isalpha(str[i]) && i < n)
        i++;
    cmd = malloc(sizeof(char) * (++i));
    ft_strlcpy(cmd, str, i);
    return (cmd);
}

char    **get_args(char *str, int n)
{
    int     i;
    char    *tmp;

    i = 0;
    tmp = ft_strdup(str);
    if (n != 0)
        tmp[n] = '\0';
    while (!ft_isalpha(*tmp))
        tmp++;
    return (ft_split(tmp, ' '));
}

t_s_cmds    *add_command(t_s_cmds *cmds, char *cmd, int i)
{
    cmds->cmd = get_cmd(cmd, i);
    // printf("CMD: %s|\n", cmds->cmd);
    cmds->args = get_args(cmd, i);
    // printf("ARG0: %s|\n", cmds->args[0]);
    // printf("ARG1: %s|\n", cmds->args[1]);
    if (i != 0)
    {
        cmds->next = malloc(sizeof(t_s_cmds));
        cmds = cmds->next;
        cmds->cmd = NULL;
        cmds->append = 0;
        cmds->next = NULL;
    }
    return (cmds);
}

t_s_cmds    *get_commands(t_m_cmds *m_cmd, char *cmd)
{
    t_s_cmds    *cmds;
    int         i;

    i = 0;
    cmds = malloc(sizeof(t_s_cmds));
    m_cmd->s_cmds = cmds;
    while (cmd[i])
    {
        if (cmd[i] == '>' || cmd[i] == '<')
        {
            cmds->append = (cmd[i] == '>') ? 1 : -1;
            if (cmd[i + 1] == '>')
                cmds->append = 2;
            cmds = add_command(cmds, cmd, i);
            cmd = cmd + i + ((cmds->append < 0) ? 1 : cmds->append);
            i = 0;
        }
        else
            i++;
    }
    if (!cmds->cmd)
        cmds = add_command(cmds, cmd, 0);
    return (m_cmd->s_cmds);
}

t_m_cmds    *get_pipe_commands(t_b_cmds *b_cmd)
{
    t_m_cmds    *cmds;
    char        **tmp;

    if (ft_strchr(b_cmd->cmd, '|'))
        tmp = ft_split(b_cmd->cmd, '|');
    else
        tmp = ft_split(b_cmd->cmd, '\0');
    if (*tmp)
    {
        cmds = malloc(sizeof(t_m_cmds));
        cmds->next = NULL;
        b_cmd->m_cmds = cmds;
    }
    while (*tmp)
    {
        cmds->cmd = *tmp;
        cmds->s_cmds = get_commands(cmds, cmds->cmd);
        tmp++;
        if (*tmp)
        {
            cmds->next = malloc(sizeof(t_m_cmds));
            cmds = cmds->next;
            cmds->next = NULL;
        }
    }
    return (b_cmd->m_cmds);
}

t_shell     *get_comma_commands(t_shell *shell)
{
    t_b_cmds    *cmds;
    char        **tmp;

    tmp = (ft_strchr(shell->line, ';')) ? ft_split(shell->line, ';') : ft_split(shell->line, '\0');
    if (*tmp)
    {
        cmds = malloc(sizeof(t_b_cmds));
        shell->cmds = cmds;
    }
    while (*tmp)
    {
        cmds->num_pipe = 0;
        cmds->next = NULL;
        cmds->cmd = *tmp;
        while (**tmp)
        {
            if (**tmp == '|')
                cmds->num_pipe++;
            (*tmp)++;
        }
        cmds->m_cmds = get_pipe_commands(cmds);
        tmp++;
        cmds->next = (*tmp) ? malloc(sizeof(t_b_cmds)) : NULL;
        cmds = cmds->next;
    }
    return (shell);
}

// --------------- TEST -------------------

// int main()
// {
//     t_shell *shell;
//     shell = malloc(sizeof(t_shell));

//     shell->line = ft_strdup("echo -n \"hello\" >> f.txt haha >> c.txt | grep h");
//     //shell = get_single_command(shell);
//     //shell = control_command(shell);
//     shell = get_comma_commands(shell);
//     printf("%s\n", shell->cmds->m_cmds->s_cmds->cmd);
// }
