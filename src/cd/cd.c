#include "cd.h"

char OLDPWD[100];

int cd(char *a)
{
    // char buf[100];
    while (*a == ' ')
        a += 1;
    int res = 1;
    if (a == NULL)
    {
        getcwd(OLDPWD, 100);
        res = chdir(getenv("HOME"));
        // printf("%s\n", getcwd(buf, 100));
    }
    else if (strcmp("-", a) == 0)
    {
        res = chdir(OLDPWD);
        // printf("%s\n", getcwd(buf, 100));
    }
    else
    {
        getcwd(OLDPWD, 100);
        res = chdir(a);
        // printf("%s\n", getcwd(buf, 100));
    }

    if (res == -1)
        res = 1;
    if (res != 0)
        fprintf(stderr, "error cd\n");
    return res;
}