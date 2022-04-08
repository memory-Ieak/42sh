#include "lexer.h"

struct lexer *lexer_get_token(struct lexer *lexer);

struct lexer *lexer_new(char *input)
{
    struct lexer *lexer = malloc(sizeof(struct lexer));
    lexer->input = input;
    lexer->pos = 0;
    lexer->current_tok = NULL;
    return lexer_get_token(lexer);
}

void lexer_free(struct lexer *lexer)
{
    if (lexer->current_tok)
        token_free(lexer->current_tok);
    free(lexer);
}

int charcmp(char *cmp, char c, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (cmp[i] == c)
            return 0;
    }
    return 1;
}

char *get_word(struct lexer *lexer)
{
    char end[] = { ';', '\n', ' ', '\0', '!' };
    size_t len = 5;
    if (lexer->input[lexer->pos] == ' ' || lexer->input[lexer->pos] == '\t')
    {
        lexer->pos++;
        return get_word(lexer);
    }
    int size = 0;
    int capacity = 1;
    char *word = malloc(sizeof(char) * capacity);
    if (!charcmp(end, lexer->input[lexer->pos + size], len))
    {
        capacity++;
        word = realloc(word, sizeof(char) * capacity);
        word[size] = lexer->input[lexer->pos + size];
        size++;
        word[size] = '\0';
        return word;
    }
    char quote = 0;
    if (lexer->input[lexer->pos] == '"' || lexer->input[lexer->pos] == '\'')
        quote = 1;
    while ((quote == 1 || charcmp(end, lexer->input[lexer->pos + size], len))
           && lexer->input[lexer->pos + size] != '\0')
    {
        capacity++;
        word = realloc(word, sizeof(char) * capacity);
        word[size] = lexer->input[lexer->pos + size];
        size++;
        if (quote == 1 && lexer->input[lexer->pos] == '\0')
            quote = 2;
        if (quote == 1
            && (lexer->input[lexer->pos + size] == '"'
                || lexer->input[lexer->pos + size] == '\''))
            quote = 0;
    }
    word[size] = '\0';
    if (quote == 2)
    {
        free(word);
        return NULL;
    }
    return word;
}

int have_equal(char *word)
{
    if (word[0] == '\"' || word[0] == '\'')
        return 1;
    while (*word != '\0')
    {
        if (*word == '=')
            return 0;
        word++;
    }

    return 1;
}

struct lexer *lexer_get_token(struct lexer *lexer)
{
    struct token tokens[] = {
        { TOKEN_IF, "if" },
        { TOKEN_THEN, "then" },
        { TOKEN_ELIF, "elif" },
        { TOKEN_ELSE, "else" },
        { TOKEN_FI, "fi" },
        { TOKEN_SEMICOLON, ";" },
        { TOKEN_NEW_LINE, "\n" },
        { TOKEN_SINGLE_QUOTE, "'" },
        { TOKEN_EOF, "\0" },
        { TOKEN_WHILE, "while" },
        { TOKEN_DO, "do" },
        { TOKEN_DONE, "done" },
        { TOKEN_OR, "||" },
        { TOKEN_AND, "&&" },
        { TOKEN_PIPELINE, "|" },
        { TOKEN_NEGATION, "!" },
        { TOKEN_FOR, "for" },
        { TOKEN_UNTIL, "until" }
        //{ TOKEN_VARIABLE, "$" }

    };
    char *word = get_word(lexer);
    while (word == NULL)
        word = get_word(lexer);
    struct token *tok = token_new(TOKEN_WORD);
    tok->value = word;
    for (unsigned i = 0; i < sizeof(tokens) / sizeof(*tokens); i++)
    {
        if (!strcmp(tokens[i].value, word))
        {
            tok->type = tokens[i].type;
            break;
        }
        /*else if (tokens[i].type == TOKEN_VARIABLE && *word == '$')
        {
            tok->type = tokens[i].type;
            break;
        }*/
    }
    if (tok->type == TOKEN_WORD && have_equal(word) == 0)
        tok->type = TOKEN_ASSIGNMENT;
    lexer->current_tok = tok;
    lexer->pos += strlen(word);
    return lexer;
}

struct token *lexer_peek(struct lexer *lexer)
{
    return lexer->current_tok;
}

struct token *lexer_pop(struct lexer *lexer)
{
    struct token *token = lexer->current_tok;
    lexer = lexer_get_token(lexer);
    return token;
}
