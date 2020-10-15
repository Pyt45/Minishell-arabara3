#include "../../includes/shell.h"


int		pwd_builtin()
{
	char	*buff;

	if ((buff = getcwd(NULL, 0)) == NULL)
	{
		perror("getcwd()");
	}
	else
	{
		ft_putstr_fd(buff, 1);
		ft_putstr_fd("\n", 1);
		free(buff);
	}
	return (1);
}