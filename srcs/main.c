#include <stdio.h>

int     main(int argc, char **argv, char **env)
{
    if (argc)
    {
        char    **envs = env;
        while (*envs)
        {
            printf("%s\n", *envs);
            envs++;
        }
    }
    return (0);
}

void    test()
{
    
}