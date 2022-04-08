#pragma once

#include <stdio.h>
#include <string.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"

void print_error(struct lexer *lexer, struct token *token,
                 enum token_type expected);
void standard_error(struct token *token);