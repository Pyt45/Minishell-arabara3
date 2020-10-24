
#include "../includes/shell.h"

void    debug_cmd(t_cmds *cmds, int i, int pos, char c){
    int j;

    printf("CHAR: %c | POS:%d | I=%d\n", c, pos, i);
    printf("CMD: %s|\n", cmds->cmd);
    j = 0;
	if (cmds->args)
		while (cmds->args[j]){
			printf("ARG %d: %s|\n--------------------------\n", j, cmds->args[j]);
			j++;
		}
	else
		puts("ARG: No arguments");
}

t_cmds      *init_cmds(t_cmds   *prev)
{
    t_cmds  *cmds;

    cmds = malloc(sizeof(t_cmds));
    cmds->start = 0;
    cmds->end = 0;
    cmds->p = 0;
    cmds->r = 0;
    cmds->append = 0;
    cmds->ret = 0;
    cmds->prev = NULL;
    if (prev)
        cmds->prev = prev;
    cmds->next = NULL;
    return (cmds);
}

char    *get_cmd(char *str, int n)
{
    int     i;
    char    *cmd;

    i = 0;
    while ((!ft_isprint(*str) || *str == ' ') && *str != '\0')
        str++;
    if (n == 0)
        n = ft_strlen(str);
    while (ft_isprint(str[i]) && str[i] != ' ' && str[i] != ';' && str[i] != '|' && i < n)
        i++;
    cmd = malloc(sizeof(char) * (++i));
    ft_strlcpy(cmd, str, i);
	if (*cmd)
    	return (cmd);
	return (NULL);
}

char    **get_args(char *str, int n)
{
    int     i;
    char    *tmp;

    i = 0;
    while ((!ft_isprint(*str) || *str == ' ') && *str != '\0')
    {
        str++;
        i++;
    }
    if (*str)
    {
        tmp = ft_strdup(str);
        if (n - i >= 0)
            tmp[n - i] = '\0';
        while (!ft_isalpha(*tmp) && *tmp)
            tmp++;
		if (*tmp)
			return (ft_split_quote(tmp, ' '));
    }
    return (NULL);
}

int        parse_pipes(t_cmds **cmds, int i, int pos,char *tmp){

    (*cmds)->cmd = get_cmd(tmp + pos, i - pos);
    (*cmds)->args = get_args(tmp + pos, i - pos);
    // debug_cmd(*cmds, i, pos, tmp[i]);
	if (!(*cmds)->args)
		return (-1);
    if (!(*cmds)->prev)
        (*cmds)->start = 1;
	// this to manage syntax error and segfault (maybe remove the end of string check)
	if (tmp[i + 1] != '\0')
    {
		(*cmds)->p = 1;
        (*cmds)->next = init_cmds(*cmds);
        (*cmds) = (*cmds)->next;
    }
	else
		return (-1);
    pos = i + 1;
    return (pos);
}

int        parse_semicolons(t_cmds **cmds, int i, int pos,char *tmp)
{
    int j;

    j = 0;
    if (tmp[i + 1] == '\0' && tmp[i] != ';')
        j = 1;
    (*cmds)->cmd = get_cmd(tmp + pos, i - pos + j);
    (*cmds)->args = get_args(tmp + pos, i - pos + j);
    // debug_cmd((*cmds), i, pos, tmp[i]);
	if (!(*cmds)->args)
		return (-1);
    if (!(*cmds)->prev)
        (*cmds)->start = 1;
    (*cmds)->end = 1;
    if (tmp[i + 1] != '\0')
    {
        (*cmds)->next = init_cmds(*cmds);
        (*cmds) = (*cmds)->next;
    }
    pos = i + 1;
    return (pos);
}

int        parse_redirections(t_cmds **cmds, int *i, int pos,char *tmp){
    (*cmds)->cmd = get_cmd(tmp + pos, *i - pos);
    (*cmds)->args = get_args(tmp + pos, *i - pos);
    debug_cmd(*cmds, *i, pos, tmp[*i]);
	if (!(*cmds)->args ||!(*cmds)->cmd)
		return (-1);
    if (!(*cmds)->prev)
        (*cmds)->start = 1;
    if (tmp[*i] == '>')
        (*cmds)->append = 1;
    else if (tmp[*i] == '<')
        (*cmds)->append = -1;
    if (tmp[*i + 1] == '>')
    {
        (*cmds)->append++;
        (*i)++;
    }
    else if (tmp[*i + 1] == '<')
    {
        (*cmds)->append--;
        (*i)++;
    }
	if ((*cmds)->append != 0)
		(*cmds)->r = 1;
    (*cmds)->next = init_cmds((*cmds));
    (*cmds) = (*cmds)->next;
    pos = *i + 1;
    return (pos);
}

t_shell     *parse_commands(t_shell *shell)
{
    t_cmds      *cmds;
    int         i;
    int         pos;
    char        *tmp;
	int			quote;
    
    pos = 0;
    i = -1;
	quote = 0;
	tmp = replace_string(shell->line, shell);
    cmds = init_cmds(NULL);
    shell->cmds = cmds;
    while (tmp[++i] && pos != -1)
    {
		if (is_quote(tmp[i], 0))
			quote = !quote ? 1 : 0;
        if (tmp[i] == '|' && !quote)
            pos = parse_pipes(&cmds, i, pos, tmp);
        else if (!quote && (tmp[i] == ';' || tmp[i + 1] == '\0'))
            pos = parse_semicolons(&cmds, i, pos, tmp);
        else if (!quote && (tmp[i] == '>' || tmp[i] == '<'))
            pos = parse_redirections(&cmds, &i, pos, tmp);
        // i++;
		if (pos == -1)
			shell->parse_err = 1;
    }
    return (shell);
}
