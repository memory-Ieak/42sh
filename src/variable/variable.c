#include "variable.h"

struct variable_list *var_init(void)
{
    struct variable_list *new_list = malloc(sizeof(struct variable_list));
    if (new_list == NULL)
        return NULL;
    new_list->size = 0;
    new_list->var = NULL;

    return new_list;
}

char *new_str(char *str)
{
    if (str)
    {
        size_t size = strlen(str);
        char *res = malloc(size + 1);
        if (res == NULL)
            return NULL;
        for (size_t i = 0; i < size; i++)
            res[i] = str[i];
        res[size] = '\0';
        return res;
    }
    return NULL;
}

struct variable *new_var(char *type, char *name, char *value)
{
    struct variable *new_var = malloc(sizeof(struct variable));
    if (new_var == NULL)
        return NULL;
    new_var->type = new_str(type);
    new_var->name = new_str(name);
    new_var->value = new_str(value);

    return new_var;
}

int add_var(struct variable_list *var_list, struct variable *var)
{
    if (change_var(var_list, var) == 1)
    {
        return 1;
    }
    struct variable **tmp;
    if (var_list->size == 0)
        tmp = malloc(sizeof(struct variable *));
    else
        tmp = realloc(var_list->var,
                      sizeof(struct variable *) * (var_list->size + 1));

    if (tmp == NULL)
        return 1;
    var_list->var = tmp;
    var_list->var[var_list->size] = var;
    var_list->size += 1;

    return 0;
}

struct variable_list *cp_var_list(struct variable_list *var_list)
{
    if (var_list != NULL)
    {
        struct variable_list *res = var_init();
        for (size_t i = 0; i < var_list->size; i++)
        {
            struct variable *var =
                new_var(var_list->var[i]->type, var_list->var[i]->name,
                        var_list->var[i]->value);
            add_var(res, var);
        }

        return res;
    }

    return NULL;
}

struct variable *get_var(struct variable_list *var_list, char *name)
{
    struct variable *res = NULL;
    if (var_list != NULL)
    {
        for (size_t i = 0; i < var_list->size; i++)
        {
            if (strcmp(var_list->var[i]->name, name) == 0)
            {
                res = var_list->var[i];
                break;
            }
        }
    }

    return res;
}

int change_var(struct variable_list *var_list, struct variable *var)
{
    if (var_list != NULL)
    {
        for (size_t i = 0; i < var_list->size; i++)
        {
            if (strcmp(var_list->var[i]->name, var->name) == 0)
            {
                struct variable *old = var_list->var[i];
                var_list->var[i] = var;
                free(old);
                return 1;
            }
        }
    }

    return 0;
}

int delete_var(struct variable_list *var_list, char *name)
{
    if (var_list != NULL)
    {
        size_t i = 0;
        while (i < var_list->size)
        {
            if (strcmp(var_list->var[i]->name, name) == 0)
            {
                free_var(var_list->var[i]);
                var_list->size -= 1;
                break;
            }
            i++;
        }
        while (i < var_list->size)
        {
            var_list->var[i] = var_list->var[i + 1];
            i++;
        }
    }

    return 0;
}

char *get_name(char *str)
{
    char *res = malloc(1);
    int i = 0;
    while (*(str + i) != '=')
    {
        res = realloc(res, i + 2);
        res[i] = *(str + i);
        i++;
    }
    res[i] = '\0';

    return res;
}

char *get_value(char *str)
{
    char *res = malloc(1);
    int i = 0;
    while (*str != '=')
        str++;
    str++;
    while (*(str + i) != '\0')
    {
        res = realloc(res, i + 2);
        res[i] = *(str + i);
        i++;
    }
    res[i] = '\0';

    return res;
}

void free_var(struct variable *var)
{
    if (var != NULL)
    {
        free(var->type);
        free(var->name);
        free(var->value);
        free(var);
    }
}

void free_var_list(struct variable_list *var_list)
{
    if (var_list != NULL)
    {
        for (size_t i = 0; i < var_list->size; i++)
            free_var(var_list->var[i]);
        free(var_list->var);
        free(var_list);
    }
}

void print_var(struct variable_list *var_list)
{
    if (var_list != NULL && var_list->size != 0)
    {
        for (size_t i = 0; i < var_list->size; i++)
        {
            printf("type: %s, name: %s, value: %s\n", var_list->var[i]->type,
                   var_list->var[i]->name, var_list->var[i]->value);
        }
    }
    else
    {
        printf("Empty list !\n");
    }
}

char *replace_string_var(struct variable_list *var_list, char *str)
{
    char *res = malloc(1);
    size_t i = 0;
    while (*str != '\0')
    {
        // It's a variable
        if (*str == '$')
        {
            str++;
            // Get name
            char *name = malloc(1);
            size_t size = 1;
            size_t j = 0;
            if (*str == '{')
                str++;
            while ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z')
                   || (*str >= '0' && *str <= '9'))
            {
                name = realloc(name, size + 1);
                size++;
                name[j] = *str;
                str++;
                j++;
            }
            name[j] = '\0';
            if (*str == '}')
                str++;

            // Get value
            struct variable *var = get_var(var_list, name);
            if (var != NULL)
            {
                char *value = var->value;
                while (*value != '\0')
                {
                    res = realloc(res, i + 1);
                    res[i] = *value;
                    i++;
                    value++;
                }
            }
            free(name);
        }
        // It's not a variable
        else
        {
            res = realloc(res, i + 1);
            res[i] = *str;
            str++;
            i++;
        }
    }
    res = realloc(res, i + 1);
    res[i] = '\0';

    return res;
}