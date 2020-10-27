/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 18:10:41 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/27 19:38:09 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// void    debug_cmd(t_cmds *cmds, int i, int pos, char c)
// {
//     int j;

//     printf("CHAR: %c | POS:%d | I=%d\n", c, pos, i);
//     printf("CMD: %s|\n", cmds->cmd);
//     j = 0;
// 	if (cmds->args)
// 		while (cmds->args[j]){
// 			printf("ARG %d: %s|\n--------------------------\n", j, cmds->args[j]);
// 			j++;
// 		}
// 	else
// 		puts("ARG: No arguments");
// }

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
	int		quote;

    i = 0;
	quote = 0;
    while ((!ft_isprint(*str) || *str == ' ') && *str != '\0')
        str++;
    if (n == 0)
        n = ft_strlen(str);
	while (ft_isprint(str[i]) && i < n)
	{
		if (is_quote(str[i], 0))
			quote = quote_activer(quote, str[i]);
		if ((str[i] == ' ' || str[i] == ';' || str[i] == '|') && !quote)
			break;
		i++;
	}
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
    while ((!ft_isprint(*str) || *str == ' ') && *str)
    {
        str++;
        i++;
    }
    if (*str)
    {
        tmp = ft_strdup(str);
        if (n - i >= 0)
            tmp[n - i] = '\0';
        // while (!ft_isalpha(*tmp) && *tmp)
        //     tmp++;
		if (*tmp)
			return (ft_split_quote(tmp, ' '));
    }
    return (NULL);
}

char	*replace_string(char *str, t_shell *shell)
{
	if (ft_strchr(str, '$'))
	    str = parse_env_var(str, shell);
	return (str);
}

void	manage_redirections(t_cmds **cmds, int *i, char *tmp)
{
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
}