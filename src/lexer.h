#ifndef LEXER_H
#define LEXER_H

#include "arr.h"
#include "parser.h"
#include <stdbool.h>

typedef enum ast_type {
    NODE_NUMBER,
    NODE_BIN_OP,
    NODE_VARIABLE,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_PRINT,
    NODE_BLOCK,
    NODE_NOOP
} ast_type;

typedef struct ast_node {
    ast_type type;
    union {
        double value;
        char* var_name;

        struct {
            struct ast_node* left;
            struct ast_node* right;
            TokenType op;
        } binary;

        struct {
            char* var_name;
            struct ast_node* value;
        } assignment;

        struct {
            struct ast_node* condition;
            struct ast_node* if_body;
            struct ast_node* else_body;
        } if_stmt;

        struct {
            struct ast_node* expression;
        } print;

        struct {
            arr_t* statements;
        } block;
    } data;
} ast_node;

typedef struct lexer_t {
    token *current;
    size_t current_index;
    arr_t *tokens;
} lexer_t;

void print_ast(ast_node *node, int indent);

ast_node *new_number_node(double value);
ast_node *new_binary_node(ast_node *left, ast_node *right, TokenType type);
ast_node *new_variable_node(const char *name);
ast_node *new_assignment_node(const char *var_name, ast_node *value);
ast_node *new_if_node(ast_node *condition, ast_node *if_body, ast_node *else_body);
ast_node *new_print_node(ast_node *expression);
ast_node *new_block_node(arr_t *statements);
ast_node *parse_comparison(lexer_t *lexer);

ast_node *build_ast_tree(arr_t* tokens);
void free_ast(ast_node *node);

lexer_t *new_lexer(arr_t *tokens);
void free_lexer(lexer_t *lexer);
void lexer_skip(lexer_t *lexer, size_t count);
void lexer_one_skip(lexer_t *lexer);
void lexer_syntax_error(lexer_t *lexer , const char* message);
bool lexer_skip_if_eq(lexer_t *lexer , TokenType type);

token *lexer_take(lexer_t *lexer);
token *lexer_look_back(lexer_t *lexer);
token *lexer_look_next(lexer_t *lexer);

ast_node *build_ast_tree(arr_t *tokens);
ast_node *parse_block(lexer_t *lexer);
ast_node *parse_statement(lexer_t *lexer);
ast_node *parse_if_statement(lexer_t *lexer);
ast_node *parse_print_statement(lexer_t *lexer);
ast_node *parse_expression(lexer_t *lexer);
ast_node *parse_comparison(lexer_t *lexer);
ast_node *parse_term(lexer_t *lexer);
ast_node *parse_factor(lexer_t *lexer);

#endif
