#include "error_parser.h"

void red()
{
    fprintf(stderr, "\033[1;31m");
}

void reset()
{
    fprintf(stderr, "\033[0m");
}

size_t get_begin(char *str, int pos)
{
    while (pos > 0 || str[pos] != '\n')
        pos--;
    return (pos < 0) ? 0 : pos;
}

size_t print_line(struct lexer *lexer)
{
    size_t i = 0;
    size_t pos = get_begin(lexer->input, lexer->pos);
    char *line = lexer->input;
    while (line[pos + i] != '\0' && line[pos + i] != '\n')
    {
        fprintf(stderr, "%c", line[pos + i]);
        i++;
    }
    fprintf(stderr, "\n");
    return lexer->pos - get_begin(lexer->input, lexer->pos);
}

void print_error(struct lexer *lexer, struct token *token,
                 enum token_type expected)
{
    fprintf(stderr, "\n");
    size_t len = print_line(lexer) - strlen(token->value);
    for (size_t i = 0; i < len; i++)
        fprintf(stderr, " ");
    red();
    fprintf(stderr, "^\n");
    fprintf(stderr, "42sh: Syntax error: \"%s\" unexpected\n",
            _print_token(token->type));
    fprintf(stderr, "Do you mean \"%s\" ?\n", _print_token(expected));
    reset();
}

void standard_error(struct token *token)
{
    red();
    fprintf(stderr, "42sh: Syntax error: \"%s\" unexpected.\n",
            _print_token(token->type));
    reset();
}