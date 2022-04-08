#define D_GNU_SOURCE

#include "eval.h"

struct ast_node *get_condition(struct ast_node *node)
{
    return node->children[0];
}

int is_break(char *str)
{
    if (fnmatch("break[;\n]\0", str, 0) == 0)
    {
        return 1;
    }
    else
        return 0;
}

int is_continue(char *str)
{
    if (fnmatch("continue[;\n]\0", str, 0) == 0)
    {
        return 1;
    }
    else
        return 0;
}

int is_exit(char *str, struct statut *stat)
{
    if (str[0] == 'e' && str[1] == 'x' && str[2] == 'i' && str[3] == 't')
        str += 4;
    else
        return 0;

    if (*str == '\n' || *str == ';' || *str == '\0')
        return 1;

    if (*str == ' ')
        str++;
    else
        return 0;
    int res = 0;
    while (*str != '\n' && *str != ';' && *str != '\0')
    {
        if (*str < '0' || *str > '9')
            return 0;
        else
        {
            res *= 10;
            res += *str - '0';
        }
        str++;
    }

    if (*str != '\0')
    {
        str++;
        if (*str != '\0')
            return 0;
    }

    stat->res = res;

    return 1;
}

int is_stat(char *str, struct statut *stat)
{
    if (is_break(str) == 1)
    {
        stat->value = 1;
    }
    else if (is_continue(str) == 1)
    {
        stat->value = 2;
    }
    else if (is_exit(str, stat) == 1)
    {
        stat->value = 3;
    }

    return stat->value;
}

int exec(char *command[])
{
    int exit_statut = 0;
    int pid;
    pid = fork();
    if (pid == 0)
    {
        execvp(command[0], command);
    }
    else
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        exit_statut = WEXITSTATUS(wstatus);
    }
    return exit_statut;
}

int cmdcmp(char *cmp, char *cmd, int s)
{
    int i = 0;
    while (cmp[i] && cmd[i])
    {
        if (cmp[i] != cmd[i])
            return 0;
        i++;
    }
    if (i == s)
        return 1;
    return 0;
}

char **char_to_string(char *command)
{
    char *saveptr = NULL;
    char **exec_command = malloc(sizeof(char *) * 20);
    int i = 0;
    for (char *str = command;; str = NULL)
    {
        char *token = strtok_r(str, " ", &saveptr);
        if (!token)
            break;
        char *str = malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(str, token);
        exec_command[i] = str;
        i++;
    }
    exec_command[i] = NULL;
    return exec_command;
}

void free_str(char **command)
{
    if (!command)
        return;
    int i = 0;
    while (command[i])
    {
        free(command[i]);
        i++;
    }
    free(command);
}

int find_command(char *command, struct variable_list *var_list,
                 struct statut *stat)
{
    if (!strcmp("echo $?", command))
        return echo_exit_code(stat->res);
    int res = 0;
    char *cmd[] = { "echo", "cd" };
    struct builtin blt[] = { { echo }, { cd } };
    int size = 2;
    char *new_command = replace_string_var(var_list, command);
    for (int i = 0; i < size; i++)
    {
        if (cmdcmp(cmd[i], new_command, strlen(cmd[i])))
        {
            res = blt[i].func(new_command + strlen(cmd[i]));
            free(new_command);
            return res;
        }
    }
    char **exec_command = char_to_string(new_command);
    res = exec(exec_command);
    free_str(exec_command);
    free(new_command);
    return res;
}

int exec_conditon(struct ast_node *node, struct variable_list *var_list,
                  struct statut *stat)
{
    int boolean = 0;
    for (size_t i = 0; i < node->nb_children; i++)
    {
        char *condition =
            replace_string_var(var_list, node->children[i]->token->value);
        if (!strcmp(condition, "true")
            || !find_command(condition, var_list, stat))
            boolean = 1;
        free(condition);
    }
    return boolean;
}

struct ast_node *eval_rule_if(struct ast_node *node,
                              struct variable_list *var_list,
                              struct statut *stat)
{
    for (size_t i = 0; i < node->nb_children; i++)
    {
        struct ast_node *current_node = node->children[i];
        if (current_node->token->type == TOKEN_ELSE)
            return node->children[i];
        if (exec_conditon(get_condition(current_node), var_list, stat))
        {
            return node->children[i];
        }
    }
    return NULL;
}

int check_condition(struct ast_node *node, struct variable_list *var_list,
                    struct statut *stat)
{
    if (exec_conditon(node->children[0], var_list, stat) == 1)
        return 1;
    else
        return 0;
}

int exec_pipe(struct ast_node *argv_left, struct ast_node *argv_right,
              struct variable_list *var_list, struct statut *stat)
{
    int des_p[2];
    if (pipe(des_p) == -1)
    {
        exit(1);
    }

    if (fork() == 0) // first fork
    {
        close(STDOUT_FILENO); // closing stdout
        dup(des_p[1]); // replacing stdout with pipe write
        close(des_p[0]); // closing pipe read
        close(des_p[1]);
        // exec("echo tata");
        eval(argv_left, var_list, stat); // fils gauche
        exit(1);
    }

    if (fork() == 0) // creating 2nd child
    {
        close(STDIN_FILENO); // closing stdin
        dup(des_p[0]); // replacing stdin with pipe read
        close(des_p[1]); // closing pipe write
        close(des_p[0]);
        // exec("tr a o");
        eval(argv_right, var_list, stat);
        exit(1);
    }

    close(des_p[0]);
    close(des_p[1]);
    wait(0);
    wait(0);
    return 0;
}

void eval(struct ast_node *node, struct variable_list *var_list,
          struct statut *stat)
{
    if (!node || stat->value == 3)
    {
        return;
    }
    else if (node->token->type == TOKEN_CONDITION)
    {
        return;
    }
    else if (node->token->type == TOKEN_COMMAND)
    {
        if (is_stat(node->token->value, stat) != 0)
            return;
        stat->res = find_command(node->token->value, var_list, stat);
    }
    else if (node->token->type == TOKEN_RULE_IF)
    {
        struct ast_node *valid = eval_rule_if(node, var_list, stat);
        struct variable_list *var_list_cp = cp_var_list(var_list);
        if (valid != NULL)
        {
            if (valid->token->type == TOKEN_ELSE)
                eval(valid->children[0], var_list_cp, stat);
            else
                eval(valid->children[1], var_list_cp, stat);
        }
    }
    else if (node->token->type == TOKEN_WHILE)
    {
        while (check_condition(node, var_list, stat) == 1)
        {
            if (stat->value == 3)
                return;
            if (stat->value == 1)
            {
                stat->value = 0;
                break;
            }
            if (stat->value == 1)
            {
                stat->value = 0;
                continue;
            }
            struct variable_list *var_list_cp = cp_var_list(var_list);
            eval(node->children[1], var_list_cp, stat);
        }
    }
    else if (node->token->type == TOKEN_UNTIL)
    {
        while (check_condition(node, var_list, stat) == 0)
        {
            if (stat->value == 3)
                return;
            if (stat->value == 1)
            {
                stat->value = 0;
                break;
            }
            if (stat->value == 1)
            {
                stat->value = 0;
                continue;
            }
            struct variable_list *var_list_cp = cp_var_list(var_list);
            eval(node->children[1], var_list_cp, stat);
        }
    }
    else if (node->token->type == TOKEN_FOR)
    {
        char *saveptr = NULL;
        for (char *str = node->children[1]->token->value;; str = NULL)
        {
            if (stat->value == 3)
                return;
            if (stat->value == 1)
            {
                stat->value = 0;
                break;
            }
            if (stat->value == 1)
            {
                stat->value = 0;
                continue;
            }
            char *value = strtok_r(str, " ", &saveptr);
            if (!value)
                break;
            struct variable *var =
                new_var("type", node->children[0]->token->value, value);
            struct variable_list *var_list_cp = cp_var_list(var_list);
            add_var(var_list_cp, var);
            eval(node->children[2], var_list_cp, stat);
        }
    }
    else if (node->token->type == TOKEN_ASSIGNMENT)
    {
        char *name = get_name(node->token->value);
        char *value = get_value(node->token->value);
        struct variable *var = new_var("type", name, value);
        add_var(var_list, var);
        free(name);
        free(value);
    }
    else if (node->token->type == TOKEN_INPUT)
    {
        eval(node->children[0], var_list, stat);
    }
    else if (node->token->type == TOKEN_LIST)
    {
        for (size_t i = 0; i < node->nb_children; i++)
        {
            if (stat->value == 3)
                return;
            if (i + 2 < node->nb_children
                && node->children[i + 1]->token->type == TOKEN_PIPELINE)
            {
                exec_pipe(node->children[i], node->children[i + 2], var_list,
                          stat);
                i += 2;
            }
            else
            {
                eval(node->children[i], var_list, stat);
            }
        }
        free_var_list(var_list);
    }
    else
    {
        for (size_t i = 0; i < node->nb_children; i++)
        {
            if (stat->value == 3)
                return;
            eval(node->children[i], var_list, stat);
        }
    }
}
