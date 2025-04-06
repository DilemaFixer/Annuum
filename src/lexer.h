#ifndef LEXER_H
#define LEXER_H

#include "arr.h"
#include "parser.h"

typedef enum ast_type {
    NODE_NUMBER,
    NODE_BIN_OP,
} ast_type;

typedef struct ast_node {
    ast_type type;

     union {
        double value;

        struct {
            struct ast_node* left;
            struct ast_node* right;
            TokenType op;
        } binary;
   } data;
} ast_node;

typedef struct lexer_t {
    token *current;
    size_t current_index;
    arr_t *tokens;
} lexer_t;

void print_ast(ast_node *node , int indent);

ast_node *new_number_node(double value);
ast_node *new_binary_node(ast_node *left , ast_node *rigth , TokenType type);

ast_node *build_ast_tree(arr_t* tokens);
void free_ast(ast_node *node);

lexer_t *new_lexer(arr_t *tokens);
void free_lexer(lexer_t *lexer);

void lexer_skip(lexer_t *lexer , size_t count);
token *lexer_take(lexer_t *lexer);
token *lexer_look_back(lexer_t *lexer);
token *lexer_look_next(lexer_t *lexer);

ast_node *build_ast_tree(arr_t *tokens);
ast_node *parse_expression(lexer_t *lexer);
ast_node *parse_term(lexer_t *lexer);
ast_node *parse_factor(lexer_t *lexer);

#endif
