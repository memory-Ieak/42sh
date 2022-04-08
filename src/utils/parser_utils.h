#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../parser/ast.h"

char *str_add(char *str1, char *str2);
int end_of_line(struct lexer *lexer);

#endif