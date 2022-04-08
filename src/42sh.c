#include <err.h>
#include <io/cstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utils/vec.h>

#include "eval/eval.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"

char *read_content_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "42sh: Can't open %s\n", filename);
        exit(127);
    }
    int capacity = 100;
    char *buffer = malloc(sizeof(char) * capacity);
    char c;
    int len = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (len == capacity)
        {
            capacity *= 2;
            buffer = realloc(buffer, sizeof(char) * capacity);
        }
        buffer[len] = c;
        len += 1;
    }
    buffer[len] = '\0';
    fclose(file);
    return buffer;
}

int run_option(int argc, char **argv)
{
    int print = 0;
    int file = 1;
    char *input;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--pretty-print"))
            print = 1;
        else if (!strcmp(argv[i], "-c"))
            file = 0;
        else
        {
            if (file)
                input = read_content_file(argv[i]);
            else
                input = argv[i];
            break;
        }
    }

    struct lexer *lexer = lexer_new(input);
    struct ast_node *root = new_node(token_new(TOKEN_INPUT));
    struct statut *stat = malloc(sizeof(struct statut));
    int res = 0;

    if (check_grammar(lexer, root) == 2)
        res = 2;
    else if (!print && root->children)
    {
        struct variable_list *var_list = var_init();
        if (stat != NULL)
        {
            stat->res = 0;
            stat->value = 0;
            eval(root, var_list, stat);
        }
        res = stat->res;
    }
    else if (print)
        pretty_print(root);

    if (file)
        free(input);
    lexer_free(lexer);
    free_ast(root);
    free(stat);

    return res;
}

int exec_line_command(char *line, struct statut *stat)
{
    struct lexer *lexer = lexer_new(line);
    struct ast_node *root = new_node(token_new(TOKEN_INPUT));

    if (check_grammar(lexer, root) == 2)
        stat->res = 2;
    else if (root->children)
    {
        struct variable_list *var_list = var_init();
        eval(root, var_list, stat);
    }
    lexer_free(lexer);
    free_ast(root);
    return stat->res;
}

/**
 * \brief Parse the command line arguments
 * \return A character stream
 */
static struct cstream *parse_args(int argc)
{
    // If launched without argument, read the standard input
    if (argc == 1)
    {
        if (isatty(STDIN_FILENO))
            return cstream_readline_create();
        return cstream_file_create(stdin, /* fclose_on_free */ false);
    }

    /*// 42sh FILENAME
    if (argc == 2)
    {
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            warn("failed to open input file");
            return NULL;
        }

        return cstream_file_create(fp, */
    /* fclose_on_free */ /*true);
}

fprintf(stderr, "Usage: %s [COMMAND]\n", argv[0]);*/
    return NULL;
}

/**
 * \brief Read and print lines on newlines until EOF
 * \return An error code
 */
enum error read_print_loop(struct cstream *cs, struct vec *line)
{
    enum error err;
    struct statut *stat = malloc(sizeof(struct statut));
    if (stat != NULL)
    {
        stat->res = 0;
        stat->value = 0;
    }
    while (true)
    {
        // Read the next character
        int c;
        if ((err = cstream_pop(cs, &c)))
        {
            free(stat);
            return err;
        }

        // If the end of file was reached, stop right there
        if (c == EOF)
            break;

        // If a newline was met, print the line
        if (c == '\n')
        {
            exec_line_command(vec_cstring(line), stat);
            vec_reset(line);
            if (stat->value == 3)
                break;
            else
                continue;
        }

        // Otherwise, add the character to the line
        vec_push(line, c);
    }
    free(stat);
    return NO_ERROR;
}

int main(int argc, char *argv[])
{
    int rc;

    // Parse command line arguments and get an input stream
    struct cstream *cs;
    if ((cs = parse_args(argc)) == NULL)
    {
        rc = run_option(argc, argv);
        goto err_parse_args;
    }

    // Create a vector to hold the current line
    struct vec line;
    vec_init(&line);

    // Run the test loop
    if (read_print_loop(cs, &line) != NO_ERROR)
    {
        rc = 1;
        goto err_loop;
    }

    // Success
    rc = 0;

err_loop:
    cstream_free(cs);
    vec_destroy(&line);
err_parse_args:
    return rc;
}
