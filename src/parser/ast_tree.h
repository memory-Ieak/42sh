#pragma once

struct ast_node
{
    struct token *token;
    struct ast_node **children;
    size_t nb_children;
};