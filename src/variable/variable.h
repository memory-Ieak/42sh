#ifndef VARIABLE_H
#define VARIABLE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct variable
{
    char *type;
    char *name;
    char *value;
};

struct variable_list
{
    size_t size;
    struct variable **var;
};

struct variable_list *var_init(void);
struct variable *new_var(char *type, char *name, char *value);
int add_var(struct variable_list *var_list, struct variable *var);
struct variable_list *cp_var_list(struct variable_list *var_list);
struct variable *get_var(struct variable_list *var_list, char *name);
int change_var(struct variable_list *var_list, struct variable *var);
int delete_var(struct variable_list *var_list, char *name);
char *get_name(char *str);
char *get_value(char *str);
void free_var_list(struct variable_list *var_list);
void free_var(struct variable *var);
void print_var(struct variable_list *var_list);
char *replace_string_var(struct variable_list *var_list, char *str);

#endif /* !VARIABLE_H */