
#include "../includes/shell.h"

char    *get_cmd(char *str, int n)
{
    int     i;
    char    *cmd;

    i = 0;
    while (!ft_isalpha(*str))
        str++;
    while (ft_isalpha(str[i]) && i < n)
        i++;
    cmd = malloc(sizeof(char) * (++i));
    ft_strlcpy(cmd, str, i);
    return (cmd);
}

char    **get_args(char *str, int n)
{
    int     i;

    i = 0;
    while (!ft_isalpha(*str))
        str++;
    return (ft_split(str, ' '));
}

t_s_cmds    *get_commands(t_m_cmds *m_cmd, char *cmd)
{
    t_s_cmds    *cmds;
    int         i;
    int         pos;
    char        *curr;
    char        *prev;
    
    i = 0;
    cmds = malloc(sizeof(t_s_cmds));
    m_cmd->s_cmds = cmds;
    while (cmd[i])
    {
        if (cmd[i] == '>')
        {
            cmds->append = 1;
            if (cmd[i + 1] == '>')
                cmds->append = 2;
            
            cmds->cmd = get_cmd(cmd, i);
            //cmds->args = get_args(cmd, i);
            i++;
            cmd = cmd + i;
        }
        else if (cmd[i] == '<')
        {
            cmds->append = -1;
            cmds->cmd = get_cmd(cmd, i);
            //cmds->args = get_args(cmd, i);
            i++;
            cmd = cmd + i;
        }
        //printf("ARG0: %s|\n", cmds->args[0]);
        //printf("ARG1: %s|\n", cmds->args[1]);
        cmds->next = malloc(sizeof(t_s_cmds));
        cmds = cmds->next;
    }
    //cmds->cmd = get_cmd(prev);
    //cmds->args = get_args(prev);
    return (m_cmd->s_cmds);
}

t_m_cmds    *get_pipe_commands(t_b_cmds *b_cmd)
{
    t_m_cmds    *cmds;

    char    **tmp = ft_split(b_cmd->cmd, '|');
    if (*tmp)
    {
        cmds = malloc(sizeof(t_m_cmds));
        cmds->s_cmds = NULL;
        cmds->next = NULL;
        b_cmd->m_cmds = cmds;
    }
    while (*tmp)
    {
        cmds->cmd = *tmp;
        cmds->s_cmds = get_commands();
        tmp++;
        if (*tmp)
        {
            cmds->next = malloc(sizeof(t_m_cmds));
            cmds = cmds->next;
            cmds->next = NULL;
        }
    }
}

t_shell     *get_comma_commands(t_shell *shell)
{
    t_b_cmds    *cmds;

    char    **tmp = ft_split(shell->line, ';');
    if (*tmp)
    {
        cmds = malloc(sizeof(t_b_cmds));
        cmds->num_pipe = 0;
        cmds->next = NULL;
        cmds->m_cmds = NULL;
        shell->cmds = cmds;
    }
    while (*tmp)
    {
        cmds->cmd = *tmp;
        while (*tmp)
        {
            if (*tmp == '|')
                cmds->num_pipe++;
            tmp++;
        }
        cmds->m_cmds = get_pipe_commands(cmds->cmd);
        tmp++;
        if (*tmp)
        {
            cmds->next = malloc(sizeof(t_b_cmds));
            cmds = cmds->next;
            cmds->num_pipe = 0;
            cmds->next = NULL;
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
    return (shell);
}

t_shell     *control_command(t_shell *shell)
{
    if (ft_strchr(shell->line, ';'))
        shell->cmds = get_commands(shell, ';');
    else if (ft_strchr(shell->line, ';'))
	{
		shell->spr = 1;
        return (get_commands(shell, ';'));
	}
    else
        return (get_single_command(shell));
}

// --------------- TEST -------------------

/* int main()
{
    t_shell *shell;
    shell = malloc(sizeof(t_shell));

    shell->line = ft_strdup("echo -n");
    shell = get_single_command(shell);
    printf("%s %s\n", shell->cmds->cmd, shell->cmds->args[1]);
} */
