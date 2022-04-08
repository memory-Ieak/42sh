#include "parser.h"

int input(struct lexer *lexer, struct ast_node *ast);
int simple_command(struct lexer *lexer, struct ast_node *ast);
int list(struct lexer *lexer, struct ast_node *ast);
int command(struct lexer *lexer, struct ast_node *ast);
int shell_command(struct lexer *lexer, struct ast_node *ast);
int rule_if(struct lexer *lexer, struct ast_node *ast);
int compound_list(struct lexer *lexer, struct ast_node *ast);
int else_clause(struct lexer *lexer, struct ast_node *ast);
int pipeline(struct lexer *lexer, struct ast_node *ast);
int rule_while(struct lexer *lexer, struct ast_node *ast);
int rule_until(struct lexer *lexer, struct ast_node *ast);
int rule_for(struct lexer *lexer, struct ast_node *ast);
int do_group(struct lexer *lexer, struct ast_node *ast);
int and_or(struct lexer *lexer, struct ast_node *ast);
int element(struct lexer *lexer, struct ast_node *ast);

// Check the grammar
int check_grammar(struct lexer *lexer, struct ast_node *ast)
{
    if (ast == NULL)
    {
        ast = create_node(TOKEN_INPUT, NULL);
    }
    int res = input(lexer, ast);
    if (res == 1)
    {
        standard_error(lexer_peek(lexer));
        return 2;
    }
    return res;
}

// Check for an input
int input(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
        return 0;
    else if (lexer_peek(lexer)->type == TOKEN_EOF)
        return 0;
    else if (compound_list(lexer, ast) == 0)
    {
        /*struct token *old = lexer_pop(lexer);
        token_free(old);*/
        if (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
            return 0;
        else if (lexer_peek(lexer)->type == TOKEN_EOF)
            return 0;
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

// Check for a list
int list(struct lexer *lexer, struct ast_node *ast)
{
    struct ast_node *node = create_node(TOKEN_LIST, NULL);
    add_child(ast, node);
    if (and_or(lexer, node) == 1)
    {
        return 1;
    }
    else
    {
        /*//ADD TOKEN_LIST
        struct ast_node *node = create_node(TOKEN_LIST, NULL);
        add_child(ast, node);*/
        while (lexer_peek(lexer)->type == TOKEN_SEMICOLON)
        {
            struct token *old = lexer_pop(lexer);
            token_free(old);
            if (and_or(lexer, ast->children[ast->nb_children - 1]) == 1)
            {
                break;
            }
        }
        return 0;
    }
}

// Check for a command
int command(struct lexer *lexer, struct ast_node *ast)
{
    if (simple_command(lexer, ast) == 0)
    {
        return 0;
    }
    else if (shell_command(lexer, ast) == 0)
        return 0;
    else
    {
        return 1;
    }
}

// Check for a simple command
int simple_command(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type == TOKEN_ASSIGNMENT)
    {
        while (lexer_peek(lexer)->type == TOKEN_ASSIGNMENT)
        {
            // prefix()
            char *str = malloc(1);
            *str = '\0';
            str = str_add(str, lexer->current_tok->value);
            struct ast_node *node = create_node(TOKEN_ASSIGNMENT, str);
            add_child(ast, node);
            struct token *old = lexer_pop(lexer);
            token_free(old);
        }
    }
    else if (lexer_peek(lexer)->type != TOKEN_WORD)
        return 1;
    char *str = malloc(1);
    *str = '\0';
    while (lexer_peek(lexer)->type == TOKEN_WORD)
    {
        // printf("%s\n", lexer_peek(lexer)->value);
        if (strlen(str) > 1)
            str = str_add(str, " ");
        str = str_add(str, lexer->current_tok->value);
        struct token *old = lexer_pop(lexer);
        token_free(old);
    }

    struct ast_node *node = create_node(TOKEN_COMMAND, str);
    add_child(ast, node);

    return 0;
}

int element(struct lexer *lexer, struct ast_node *ast)
{
    ast = ast;
    if (lexer_peek(lexer)->type != TOKEN_WORD)
        return 1;
    struct token *old = lexer_pop(lexer);
    token_free(old);

    return 0;
}

// Check for a shell command
int shell_command(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type == TOKEN_IF)
    {
        if (rule_if(lexer, ast) == 1)
            return 1;
    }
    else if (lexer_peek(lexer)->type == TOKEN_WHILE)
    {
        if (rule_while(lexer, ast) == 1)
        {
            return 1;
        }
    }
    else if (lexer_peek(lexer)->type == TOKEN_UNTIL)
    {
        if (rule_until(lexer, ast) == 1)
            return 1;
    }
    else if (lexer_peek(lexer)->type == TOKEN_FOR)
    {
        if (rule_for(lexer, ast) == 1)
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

// Check for a rule if
// Example : If Compound; Then; List; [Else clause] Fi;
int rule_if(struct lexer *lexer, struct ast_node *ast)
{
    // If
    if (lexer_peek(lexer)->type != TOKEN_IF)
    {
        return 1;
    }
    // ADD RULE IF
    struct ast_node *node = create_node(TOKEN_RULE_IF, NULL);
    add_child(ast, node);
    ast = ast->children[ast->nb_children - 1];
    // ADD IF
    node = create_node(TOKEN_IF, NULL);
    add_child(ast, node);
    struct token *old = lexer_pop(lexer);
    token_free(old);
    // struct ast_node *cpd = ast->children[ast->nb_children - 1];
    /*// !
    if (lexer_peek(lexer)->type == TOKEN_NEGATION)
    {
        struct ast_node *node = create_node(TOKEN_NEGATION, NULL);
        add_child(cpd, node);
        cpd = node;
        old = lexer_pop(lexer);
        token_free(old);
    }*/
    // ADD IF
    /*node = create_node(TOKEN_CONDITION, NULL);
    add_child(ast->children[0], node);*/
    // Compound
    if (compound_list(lexer, ast->children[ast->nb_children - 1]) == 1)
    {
        return 1;
    }
    // Then
    if (lexer_peek(lexer)->type != TOKEN_THEN)
    {
        // print_error(lexer, lexer_peek(lexer), TOKEN_THEN);
        return 1;
    }
    old = lexer_pop(lexer);
    token_free(old);
    /*// \n
    while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
    {
        old = lexer_pop(lexer);
        token_free(old);
    }*/
    // Compound
    if (compound_list(lexer, ast->children[ast->nb_children - 1]) == 1)
    {
        return 1;
    }
    // Else if or else
    if (lexer_peek(lexer)->type == TOKEN_ELIF
        || lexer_peek(lexer)->type == TOKEN_ELSE)
    {
        if (else_clause(lexer, ast) == 1)
            return 1;
    }
    // Fi
    if (lexer_peek(lexer)->type != TOKEN_FI)
    {
        // print_error(lexer, lexer_peek(lexer), TOKEN_FI);
        return 1;
    }
    old = lexer_pop(lexer);
    token_free(old);

    return 0;
}

// Check for an else clause
int else_clause(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type == TOKEN_ELSE)
    {
        // ADD ELSE
        struct ast_node *node = create_node(TOKEN_ELSE, NULL);
        add_child(ast, node);
        struct token *old = lexer_pop(lexer);
        token_free(old);
        // Compound
        if (compound_list(lexer, ast->children[ast->nb_children - 1]) == 1)
        {
            return 1;
        }
    }
    else if (lexer_peek(lexer)->type == TOKEN_ELIF)
    {
        // ADD ELIF
        struct ast_node *node = create_node(TOKEN_ELIF, NULL);
        add_child(ast, node);
        struct token *old = lexer_pop(lexer);
        token_free(old);
        /*struct ast_node *cpd = ast->children[ast->nb_children - 1];
        // !
        if (lexer_peek(lexer)->type == TOKEN_NEGATION)
        {
            struct ast_node *node = create_node(TOKEN_NEGATION, NULL);
            add_child(cpd, node);
            cpd = node;
            old = lexer_pop(lexer);
            token_free(old);
        }*/
        // Compound
        if (compound_list(lexer, node) == 1)
        {
            return 1;
        }
        // Then
        if (lexer_peek(lexer)->type != TOKEN_THEN)
        {
            // print_error(lexer, lexer_peek(lexer), TOKEN_THEN);
            return 1;
        }
        old = lexer_pop(lexer);
        token_free(old);
        /*// \n
        while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
        {
            old = lexer_pop(lexer);
            token_free(old);
        }*/
        // Compound
        if (compound_list(lexer, ast->children[ast->nb_children - 1]) == 1)
        {
            return 1;
        }
        // Else if or else
        if (lexer_peek(lexer)->type == TOKEN_ELIF
            || lexer_peek(lexer)->type == TOKEN_ELSE)
        {
            if (else_clause(lexer, ast) == 1)
                return 1;
        }
    }
    return 0;
}

// Check for a compound list
int compound_list(struct lexer *lexer, struct ast_node *ast)
{
    while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
    {
        struct token *old = lexer_pop(lexer);
        token_free(old);
    }
    struct ast_node *node = create_node(TOKEN_LIST, NULL);
    add_child(ast, node);
    if (and_or(lexer, ast->children[ast->nb_children - 1]) == 1)
    {
        return 1;
    }

    while (lexer_peek(lexer)->type == TOKEN_NEW_LINE
           || lexer_peek(lexer)->type == TOKEN_SEMICOLON)
    {
        struct token *old = lexer_pop(lexer);
        token_free(old);
        while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
        {
            struct token *old = lexer_pop(lexer);
            token_free(old);
        }
        if (and_or(lexer, ast->children[ast->nb_children - 1]) == 1)
            break;
    }

    return 0;
}

// Check for a pipeline
int pipeline(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type == TOKEN_NEGATION)
    {
        struct token *old = lexer_pop(lexer);
        token_free(old);
    }
    if (command(lexer, ast) == 1)
        return 1;
    while (lexer_peek(lexer)->type == TOKEN_PIPELINE)
    {
        struct ast_node *pipe_node = create_node(TOKEN_PIPELINE, NULL);
        add_child(ast, pipe_node);
        struct token *old = lexer_pop(lexer);
        token_free(old);
        while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
        {
            old = lexer_pop(lexer);
            token_free(old);
        }
        if (command(lexer, ast) == 1)
            return 1;
    }

    return 0;
}

// Check for a loop while
int rule_while(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type != TOKEN_WHILE)
    {
        return 1;
    }
    struct ast_node *while_node = create_node(TOKEN_WHILE, NULL);
    add_child(ast, while_node);
    struct token *old = lexer_pop(lexer);
    token_free(old);
    /*// !
    struct ast_node *node = while_node;

    if (lexer_peek(lexer)->type == TOKEN_NEGATION)
    {
        struct ast_node *node = create_node(TOKEN_NEGATION, NULL);
        add_child(while_node, node);
        old = lexer_pop(lexer);
        token_free(old);
    }*/
    if (compound_list(lexer, while_node) == 1)
    {
        return 1;
    }
    if (lexer_peek(lexer)->type == TOKEN_DO)
    {
        if (do_group(lexer, while_node) == 1)
        {
            return 1;
        }
    }

    return 0;
}

// Check for a loop while
int rule_until(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type != TOKEN_UNTIL)
        return 1;
    struct ast_node *until_node = create_node(TOKEN_UNTIL, NULL);
    add_child(ast, until_node);
    struct token *old = lexer_pop(lexer);
    token_free(old);

    // struct ast_node *node = until_node;

    /*// !
    if (lexer_peek(lexer)->type == TOKEN_NEGATION)
    {
        struct ast_node *node = create_node(TOKEN_NEGATION, NULL);
        add_child(until_node, node);
        old = lexer_pop(lexer);
        token_free(old);
    }*/
    if (compound_list(lexer, until_node) == 1)
        return 1;
    if (lexer_peek(lexer)->type == TOKEN_DO)
    {
        if (do_group(lexer, until_node) == 1)
            return 1;
    }

    return 0;
}

// Check for a do group
int do_group(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type != TOKEN_DO)
    {
        return 1;
    }
    struct token *old = lexer_pop(lexer);
    token_free(old);
    if (compound_list(lexer, ast) == 1)
    {
        return 1;
    }
    if (lexer_peek(lexer)->type != TOKEN_DONE)
    {
        return 1;
    }
    old = lexer_pop(lexer);
    token_free(old);

    return 0;
}

// Check for a loop for
int rule_for(struct lexer *lexer, struct ast_node *ast)
{
    if (lexer_peek(lexer)->type != TOKEN_FOR)
        return 1;
    struct ast_node *for_node = create_node(TOKEN_FOR, NULL);
    add_child(ast, for_node);
    struct token *old = lexer_pop(lexer);
    token_free(old);
    if (lexer_peek(lexer)->type != TOKEN_WORD)
        return 1;
    struct ast_node *node = create_node(TOKEN_WORD, lexer_peek(lexer)->value);
    add_child(for_node, node);
    old = lexer_pop(lexer);
    free(old); // need token->value, can't use token_free(token);

    if (lexer_peek(lexer)->type == TOKEN_NEW_LINE
        || strcmp(lexer_peek(lexer)->value, "in") == 0)
    {
        while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
        {
            old = lexer_pop(lexer);
            token_free(old);
        }
        if (strcmp(lexer_peek(lexer)->value, "in") != 0)
            return 1;
        old = lexer_pop(lexer);
        token_free(old);
        char *word = calloc(100, sizeof(char));
        int word_ind = 0;
        while (lexer_peek(lexer)->type == TOKEN_WORD)
        {
            int i = 0;
            while (lexer_peek(lexer)->value[i])
            {
                word[word_ind + i] = lexer_peek(lexer)->value[i];
                i++;
            }
            word[word_ind + i] = ' ';
            word_ind += i + 1;
            old = lexer_pop(lexer);
            token_free(old);
        }
        node = create_node(TOKEN_WORD, word);
        add_child(for_node, node);
        if (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
        {
            old = lexer_pop(lexer);
            token_free(old);
        }
        else if (lexer_peek(lexer)->type == TOKEN_SEMICOLON)
        {
            old = lexer_pop(lexer);
            token_free(old);
        }
        else
            return 1;
    }
    else if (lexer_peek(lexer)->type == TOKEN_SEMICOLON)
    {
        old = lexer_pop(lexer);
        token_free(old);
    }
    /*else
        return 0;*/

    while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
    {
        struct token *old = lexer_pop(lexer);
        token_free(old);
    }
    if (lexer_peek(lexer)->type == TOKEN_DO)
    {
        if (do_group(lexer, for_node) == 1)
            return 1;
    }

    return 0;
}

// Check for and_or operators
int and_or(struct lexer *lexer, struct ast_node *ast)
{
    if (pipeline(lexer, ast) == 1)
        return 1;
    while (lexer_peek(lexer)->type == TOKEN_AND
           || lexer_peek(lexer)->type == TOKEN_OR)
    {
        struct token *old = lexer_pop(lexer);
        token_free(old);
        while (lexer_peek(lexer)->type == TOKEN_NEW_LINE)
        {
            struct token *old = lexer_pop(lexer);
            token_free(old);
        }
        if (pipeline(lexer, ast) == 1)
            return 1;
    }

    return 0;
}

// Check for prefix
int prefix(struct lexer *lexer, struct ast_node *ast)
{
    ast = ast;
    if (lexer_peek(lexer)->type == TOKEN_ASSIGNMENT)
    {
        struct token *old = lexer_pop(lexer);
        token_free(old);
        return 0;
    }

    return 1;
}