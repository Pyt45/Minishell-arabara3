#include "../includes/shell.h"

int     exec_commands(t_shell *shell)
{
    if (!ft_strcmp(shell->cmds->cmd, "env"))
        return (env_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "cd"))
        return (cd_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "pwd"))
        return (pwd_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "exit"))
        return (exit_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "export"))
        return (export_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "unset"))
        return (unset_builtin(shell));
    if (!ft_strcmp(shell->cmds->cmd, "echo"))
        return (echo_builtin(shell));
    return (0);
}

int     excute_command_by_order(t_shell *shell)
{
    shell = control_command(shell);
    if (shell->cmds->cmd != NULL)
        return (exec_commands(shell));
    return (1);
}