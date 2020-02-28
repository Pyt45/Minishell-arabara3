#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int     main()
{
    int     pid;
    int     fds[2];

    pipe(fds);
    if ((pid = fork()) == 0)
    {
        dup2(fds[0], 0);
        close(fds[0]);
        close(fds[1]);
        char    *argv[] = {"/usr/bin/sort", NULL};
        if (execve(argv[0], argv, NULL) < 0)
            exit(0);
    }
    else
    {
        close(fds[0]);
        write(fds[1], "pear\napple\nbanana", 17);
        close(fds[1]);
    }
    int status;
    waitpid(pid, &status, 0);
}