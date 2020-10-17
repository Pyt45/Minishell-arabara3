#include "../../includes/shell.h"


int		pwd_builtin()
{
	char	*buff;

	if ((buff = getcwd(NULL, 0)) == NULL)
		print_error("pwd", 0, 0);
	else
	{
		ft_putstr_fd(buff, 1);
		ft_putstr_fd("\n", 1);
		free(buff);
	}
	return (0);
}