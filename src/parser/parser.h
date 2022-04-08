#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>

#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/parser_utils.h"
#include "ast.h"
#include "error_parser.h"

int check_grammar(struct lexer *lexer, struct ast_node *ast);

#endif