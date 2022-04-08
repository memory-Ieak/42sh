#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lexer/token.h"
#include "../utils/queue.h"
#include "ast_tree.h"

struct ast_node *new_node(struct token *token);
int add_child(struct ast_node *parent, struct ast_node *child);
int free_ast(struct ast_node *root);
void pretty_print(const struct ast_node *root);
struct ast_node *create_node(enum token_type type, char *value);
void ast_print(struct ast_node *root);

#endif /* !AST_H */