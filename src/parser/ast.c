#include "ast.h"

struct ast_node *new_node(struct token *token)
{
    struct ast_node *new = malloc(sizeof(struct ast_node));
    if (new == NULL)
        return NULL;
    new->token = token;
    new->children = NULL;
    new->nb_children = 0;
    return new;
}

struct ast_node *create_node(enum token_type type, char *value)
{
    struct token *token = token_new(type);
    token->value = value;
    struct ast_node *ast = new_node(token);
    return ast;
}

int add_child(struct ast_node *parent, struct ast_node *child)
{
    struct ast_node **tmp;
    if (parent->nb_children == 0)
        tmp = malloc(sizeof(struct ast_node *));
    else
        tmp = realloc(parent->children,
                      sizeof(struct ast_node *) * (parent->nb_children + 1));

    if (tmp == NULL)
        return 1;
    parent->children = tmp;
    parent->children[parent->nb_children] = child;
    parent->nb_children += 1;
    return 0;
}

int add_child_token(struct ast_node *parent, struct token *token)
{
    return add_child(parent, new_node(token));
}

int free_ast(struct ast_node *root)
{
    int res = 0;
    if (root == NULL)
        return 1;
    for (size_t i = 0; i < root->nb_children; i++)
    {
        if (free_ast(root->children[i]) == 1)
            res = 1;
    }
    token_free(root->token);
    free(root->children);
    free(root);
    return res;
}

int empty_node(const struct ast_node *node, char *type)
{
    if (node->nb_children == 0)
    {
        printf("\"%s\" empty !?\n", type);
        return 1;
    }
    return 0;
}

void pretty_print_(const struct ast_node *node)
{
    if (node->token->type == TOKEN_LIST)
    {
        empty_node(node, "TOKEN_LIST");
        for (size_t i = 0; i < node->nb_children; i++)
            pretty_print_(node->children[i]);
    }
    else if (node->token->type == TOKEN_RULE_IF)
    {
        empty_node(node, "TOKEN_RULE_IF");
        for (size_t i = 0; i < node->nb_children; i++)
            pretty_print_(node->children[i]);
    }
    else if (node->token->type == TOKEN_IF)
    {
        if (empty_node(node, "TOKEN_IF") == 0 && node->nb_children == 2)
        {
            printf("if { command ");
            pretty_print_(node->children[0]);
            printf(" }; then { ");
            pretty_print_(node->children[1]);
            printf(" }");
        }
        else
            printf("--> TOKEN_IF doesn t have 2 childrens !!\n");
    }
    else if (node->token->type == TOKEN_ELIF)
    {
        if (empty_node(node, "TOKEN_ELIF") == 0 && node->nb_children == 2)
        {
            printf("; elif { command ");
            pretty_print_(node->children[0]);
            printf(" }; then { ");
            pretty_print_(node->children[1]);
            printf(" }");
        }
        else
            printf("--> TOKEN_ELIF doesn t have 2 childrens !!\n");
    }
    else if (node->token->type == TOKEN_ELSE)
    {
        if (empty_node(node, "TOKEN_ELSE") == 0)
        {
            printf("; else { ");
            for (size_t i = 0; i < node->nb_children; i++)
            {
                pretty_print_(node->children[i]);
                printf(" ");
            }
            printf("}");
        }
    }
    else if (node->token->type == TOKEN_CONDITION)
    {
        empty_node(node, "TOKEN_CONDITION");
        for (size_t i = 0; i < node->nb_children; i++)
            pretty_print_(node->children[i]);
    }
    else if (node->token->type == TOKEN_COMMAND)
    {
        printf("%s", node->token->value);
    }
    else
        printf("token unknown !!\n");
}

void pretty_print(const struct ast_node *root)
{
    if (root == NULL)
        printf("Empty tree\n");
    else if (root->token->type != TOKEN_INPUT)
        printf("The root is not a \"TOKEN_INPUT\"\n");
    else if (root->nb_children == 0
             || root->children[0]->token->type != TOKEN_LIST)
        printf("your root doesn't contain the child \"TOKEN_LIST\"\n");
    else
    {
        pretty_print_(root->children[0]);
        puts("");
    }
}

void ast_print(struct ast_node *root)
{
    struct queue *q = queue_init();
    q = queue_add(q, root);
    q = queue_add(q, NULL);
    while (!queue_is_empty(q))
    {
        struct ast_node *node = queue_head(q);
        q = queue_pop(q);
        if (!node)
        {
            printf("\n");
            if (!queue_is_empty(q))
                q = queue_add(q, NULL);
        }
        else
        {
            for (size_t i = 0; i < node->nb_children; i++)
            {
                printf("fils(%s)->%s  ", print_token(node->token),
                       print_token(node->children[i]->token));
                q = queue_add(q, node->children[i]);
            }
        }
    }
}
