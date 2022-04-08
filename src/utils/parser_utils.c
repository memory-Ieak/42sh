#include "parser_utils.h"

#include <string.h>

char *str_add(char *str1, char *str2)
{
    size_t size = strlen(str1) + strlen(str2) + 2;
    char *new_str = malloc(size);
    if (new_str == NULL)
        return str1;
    strcat(strcat(strcpy(new_str, str1), ""), str2);
    free(str1);
    return new_str;
}

int end_of_line(struct lexer *lexer)
{
    if (lexer_peek(lexer)->type == TOKEN_SEMICOLON
        || lexer_peek(lexer)->type == TOKEN_NEW_LINE
        || lexer_peek(lexer)->type == TOKEN_EOF)
        return 0;
    else
        return 1;
}