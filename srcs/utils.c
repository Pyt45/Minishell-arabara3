
#include "../includes/shell.h"

// MODES: 1:X 2:W 4:R
int     ft_access(char *path, int mode)
{
    struct stat fileStat;

    if(stat(path, &fileStat) < 0)
        return (0);
    if (mode == 1)
        return (fileStat.st_mode & S_IXUSR ? 1 : 0);
    else if (mode == 2)
        return (fileStat.st_mode & S_IWUSR ? 1 : 0);
    else if (mode == 4)
        return (fileStat.st_mode & S_IRUSR ? 1 : 0);
    return (0);
}

//      ---------------- TEST   ------------------

// int main()
// {
//     printf("%d\n", ft_access("/bin/find", 1));
// }