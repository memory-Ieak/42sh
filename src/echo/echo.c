#include "echo.h"

int my_strlen(char *a)
{
    int i = 0;
    while (a[i] != '\0')
        i += 1;
    return i;
}

int is_n(char *a, int pos)
{
    int res = 0;
    if (my_strlen(a) >= 2 && a[pos] == '-' && a[pos + 1] == 'n')
    {
        if (a[pos + 2] == ' ')
            res = 1;
        else
            res = -1;
    }
    return res;
}

int is_e(char *a, int pos)
{
    int res = 0;
    if (my_strlen(a) >= 2 && a[pos] == '-' && a[pos + 1] == 'e')
    {
        if (a[pos + 2] == ' ')
            res = 1;
        else
            res = -1;
    }
    return res;
}

int is_new_line(char *a, int pos)
{
    int res = 0;
    if (a[pos] == '\\' && a[pos + 1] == 'n')
        res = 1;
    return res;
}

int is_double_slash(char *a, int pos)
{
    int res = 0;
    if (a[pos] == '\\' && a[pos + 1] == '\\')
        res = 1;
    return res;
}

int is_tab(char *a, int pos)
{
    int res = 0;
    if (a[pos] == '\\' && a[pos + 1] == 't')
        res = 1;
    return res;
}

int echo(char *argv)
{
    if (argv == NULL)
        return 0;
    while (*argv == ' ')
        argv++;
    int n = is_n(argv, 0);
    int e = is_e(argv, 0);
    if (n == -1 || e == -1)
        return 1;

    int pos = (n + e) * 3;
    while (is_n(argv, pos) >= 1 || is_e(argv, pos) >= 1)
    {
        if (is_n(argv, pos) == 1)
            n = 1;
        if (is_e(argv, pos) == 1)
            e = 1;
        pos += 3;
    }
    while (argv[pos] != '\0')
    {
        if (e == 1)
        {
            if (is_double_slash(argv, pos) == 1)
            {
                putchar('\\');
                pos += 1;
            }
            else if (is_new_line(argv, pos) == 1)
            {
                putchar('\n');
                pos += 1;
            }
            else if (is_tab(argv, pos) == 1)
            {
                putchar('\t');
                pos += 1;
            }
            else if (argv[pos] != '\'' && argv[pos] != '"')
                putchar(argv[pos]);
        }
        else if (argv[pos] != '\'' && argv[pos] != '\"')
            putchar(argv[pos]);
        pos += 1;
    }
    if (n == 0)
        putchar('\n');
    return 0;
}

int echo_exit_code(int exit_code)
{
    printf("%d\n", exit_code);
    return 0;
}