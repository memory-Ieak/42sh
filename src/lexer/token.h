#pragma once

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum token_type
{
    TOKEN_IF, // AST
    TOKEN_THEN,
    TOKEN_ELIF, // AST
    TOKEN_ELSE, // AST
    TOKEN_FI,
    TOKEN_SEMICOLON,
    TOKEN_NEW_LINE,
    TOKEN_SINGLE_QUOTE,
    TOKEN_WORD, // AST
    TOKEN_LIST, // AST
    TOKEN_RULE_IF, // AST
    TOKEN_COMMAND, // AST
    TOKEN_CONDITION, // AST
    TOKEN_INPUT, // AST
    TOKEN_WHILE, // AST
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_PIPELINE,
    TOKEN_NEGATION, // AST
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_FOR, // AST
    TOKEN_UNTIL, // AST
    // TOKEN_VARIABLE,
    TOKEN_ASSIGNMENT, // AST
    TOKEN_EOF,
    TOKEN_ERROR
};

struct token
{
    enum token_type type;
    char *value;
};

/**
 * \brief Allocate a new token
 */
struct token *token_new(enum token_type type);

/**
 * \brief Frees a token
 */
void token_free(struct token *token);

/**
 * \brief Print a token
 */
// void print_token(enum token_type type);
char *print_token(struct token *token);
char *_print_token(enum token_type type);
