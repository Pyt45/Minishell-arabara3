
#include "../../includes/shell.h"

void	erase_file_debug()
{
	FILE *f;
	int fd;

	f = fopen("debug.txt", "w");
	fd = fileno(f);
	ft_putstr_fd("", fd);
	fclose(f);
}

void	write_to_file(char *s, char *num, int end)
{
	FILE *f;
	int fd;

	f = fopen("debug.txt", "a");
	fd = fileno(f);
	ft_putstr_fd(s, fd);
	if (num)
		ft_putstr_fd(num, fd);
	if (end)
		ft_putstr_fd("\n-------\n", fd);
	fclose(f);
}

void	debug_cmd(t_cmds *cmds, int i, int pos, char c)
{
	int j;

	printf("CHAR: %c | POS:%d | I=%d\n", c, pos, i);
	printf("CMD: %s| APPEND: %d\n", cmds->cmd, cmds->append);
	j = 0;
	if (cmds->args)
		while (cmds->args[j]){
			printf("ARG %d: %s|\n--------------------------\n", j, cmds->args[j]);
			j++;
		}
	else
		puts("ARG: No arguments");
}

void		print_cmds(t_cmds *cmds)
{
	while(cmds)
	{
		write_to_file(" CMD: ", cmds->cmd, 0);
		cmds = cmds->next;
	}
	write_to_file("", "", 1);
}
