#include "token.h"

struct token *token_new(enum token_type type)
{
    struct token *new = malloc(sizeof(struct token));
    new->type = type;
    new->value = NULL;
    return new;
}

void token_free(struct token *token)
{
    free(token->value);
    free(token);
}

char *print_token(struct token *token)
{
    if (token == NULL)
        return "null";
    else
    {
        switch (token->type)
        {
        case TOKEN_IF:
            return "if";
            break;
        case TOKEN_THEN:
            return "then";
            break;
        case TOKEN_ELIF:
            return "elif";
            break;
        case TOKEN_ELSE:
            return "else";
            break;
        case TOKEN_FI:
            return "fi";
            break;
        case TOKEN_SEMICOLON:
            return ";";
            break;
        case TOKEN_NEW_LINE:
            return "\n";
            break;
        case TOKEN_SINGLE_QUOTE:
            return "'";
            break;
        case TOKEN_WORD:
            return token->value;
            break;
        case TOKEN_INPUT:
            return "input";
            break;
        case TOKEN_CONDITION:
            return "condition";
            break;
        case TOKEN_RULE_IF:
            return "rule_if";
            break;
        case TOKEN_LIST:
            return "list";
            break;
        case TOKEN_COMMAND:
            return token->value;
            break;
        case TOKEN_WHILE:
            return "while";
            break;
        case TOKEN_DO:
            return "do";
            break;
        case TOKEN_DONE:
            return "done";
            break;
        case TOKEN_PIPELINE:
            return "pipeline";
            break;
        case TOKEN_NEGATION:
            return "negation";
            break;
        case TOKEN_AND:
            return "and";
            break;
        case TOKEN_OR:
            return "or";
            break;
        case TOKEN_FOR:
            return "for";
            break;
        case TOKEN_UNTIL:
            return "until";
            break;
        case TOKEN_EOF:
            return "EOF";
            break;
        case TOKEN_ERROR:
            return "ERROR";
            break;
        default:
            return "token unrecognized!";
            break;
        }
    }
}

char *_print_token(enum token_type type)
{
    switch (type)
    {
    case TOKEN_IF:
        return "if";
        break;
    case TOKEN_THEN:
        return "then";
        break;
    case TOKEN_ELIF:
        return "elif";
        break;
    case TOKEN_ELSE:
        return "else";
        break;
    case TOKEN_FI:
        return "fi";
        break;
    case TOKEN_SEMICOLON:
        return ";";
        break;
    case TOKEN_NEW_LINE:
        return "\\n";
        break;
    case TOKEN_SINGLE_QUOTE:
        return "'";
        break;
    case TOKEN_INPUT:
        return "input";
        break;
    case TOKEN_RULE_IF:
        return "rule_if";
        break;
    case TOKEN_WORD:
        return "word";
        break;
    case TOKEN_LIST:
        return "list";
        break;
    case TOKEN_WHILE:
        return "while";
        break;
    case TOKEN_DO:
        return "do";
        break;
    case TOKEN_DONE:
        return "done";
        break;
    case TOKEN_PIPELINE:
        return "pipeline";
        break;
    case TOKEN_NEGATION:
        return "negation";
        break;
    case TOKEN_AND:
        return "and";
        break;
    case TOKEN_OR:
        return "or";
        break;
    case TOKEN_FOR:
        return "for";
        break;
    case TOKEN_UNTIL:
        return "until";
        break;
    case TOKEN_EOF:
        return "EOF";
        break;
    case TOKEN_ERROR:
        return "ERROR";
        break;
    default:
        return "token unrecognized!";
        break;
    }
}