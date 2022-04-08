#pragma once

#include <fnmatch.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../cd/cd.h"
#include "../echo/echo.h"
#include "../lexer/token.h"
#include "../parser/ast_tree.h"
#include "../variable/variable.h"

struct builtin
{
    int (*func)(char *);
};

struct statut
{
    int res; // exit value
    int value; // anything = 0, break = 1, continue = 2, exit = 3
};

void eval(struct ast_node *node, struct variable_list *var_list,
          struct statut *stat);
int find_command(char *command, struct variable_list *var_list,
                 struct statut *stat);
int exec(char **command);