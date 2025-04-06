#include "lexer.h"
#include "arr.h"
#include "logger.h"
#include "parser.h"

ast_node *new_number_node(double value) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (number type)");

  node->data.value = value;
  node->type = NODE_NUMBER;
  return node;
}

ast_node *new_binary_node(ast_node *left, ast_node *right, TokenType type) {
  if (!left)
    elog("Can't create binary node with null ptr on left ast node");
  if (!right)
    elog("Can't create binary node with null ptr on right ast node");

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (binary type)");

  node->type = NODE_BIN_OP;
  node->data.binary.left = left;
  node->data.binary.right = right;
  node->data.binary.op = type;

  return node;
}

void free_ast(ast_node *node) {
  if (!node)
    return;

  switch (node->type) {
  case NODE_BIN_OP:
    free_ast(node->data.binary.left);
    free_ast(node->data.binary.right);
    break;

  default:
    break;
  }

  free(node);
}

lexer_t *new_lexer(arr_t *tokens) {
  if (!tokens)
    elog("Can't create lexer with null ptr on tokens arr");
  if (tokens->size == 0)
    elog("Can't create lexer with empty list of tokens");
  if (tokens->data == NULL)
    elog("Can't create lexer with tokens arr with null ptr on arr_t -> items ");

  lexer_t *lexer = (lexer_t *)malloc(sizeof(lexer_t));
  if (!lexer)
    elog("Error allocation memory for lexer_t struct");

  lexer->current_index = 0;
  lexer->tokens = tokens;
  lexer->current = (token *)arr_get(tokens, 0);

  return lexer;
}

void free_lexer(lexer_t *lexer) {
  if (!lexer)
    return;

  free(lexer);
}

void lexer_skip(lexer_t *lexer, size_t count) {
  if (!lexer)
    elog("Can't skip lexer token, ptr on it is null");
  if (count == 0)
    elog("Can't skip zero tokens in lexer");

  if (lexer->current_index + count >= lexer->tokens->size) {
    count = lexer->tokens->size - lexer->current_index - 1;
    if (count <= 0)
      return;
  }

  lexer->current_index += count;
  lexer->current = arr_get(lexer->tokens, lexer->current_index);
}

token *lexer_take(lexer_t *lexer) {
  if (!lexer)
    elog("Can't take next token in lexer by null ptr on it");
  if (lexer->current_index + 1 >= lexer->tokens->size)
    return NULL;

  lexer->current_index++;
  lexer->current = arr_get(lexer->tokens, lexer->current_index);
  return lexer->current;
}

token *lexer_look_back(lexer_t *lexer) {
  if (!lexer)
    elog("Can't look back at lexer by null ptr on it");

  if (lexer->current_index == 0)
    return NULL;

  return arr_get(lexer->tokens, lexer->current_index - 1);
}

token *lexer_look_next(lexer_t *lexer) {
  if (!lexer)
    elog("Can't look next at lexer by null ptr on it");

  if (lexer->current_index + 1 >= lexer->tokens->size)
    return NULL;

  return arr_get(lexer->tokens, lexer->current_index + 1);
}

void print_ast(ast_node *node, int indent) {
  if (!node) {
    return;
  }

  for (int i = 0; i < indent; i++) {
    printf("  ");
  }

  switch (node->type) {
  case NODE_NUMBER:
    printf("NUMBER: %g\n", node->data.value);
    break;

  case NODE_BIN_OP:
    printf("BINARY_OP: ");
    switch (node->data.binary.op) {
    case TOKEN_PLUS:
      printf("+\n");
      break;
    case TOKEN_MINUS:
      printf("-\n");
      break;
    case TOKEN_MULTIPLY:
      printf("*\n");
      break;
    case TOKEN_DIVIDE:
      printf("/\n");
      break;
    default:
      printf("UNKNOWN (%d)\n", node->data.binary.op);
      break;
    }

    print_ast(node->data.binary.left, indent + 1);
    print_ast(node->data.binary.right, indent + 1);
    break;

  default:
    printf("UNKNOWN NODE TYPE (%d)\n", node->type);
    break;
  }
}

ast_node *build_ast_tree(arr_t *tokens) {
  if (!tokens)
    elog("Can't parse ast tree from null ptr on arr");
  if (tokens->size == 0)
    elog("Can't parse ast tree from empty arr");
  if (!tokens->data)
    elog(
        "Can't parse ast tree from arr, ptr on data (arr_t -> **data) is null");

  lexer_t *lexer = new_lexer(tokens);
  ast_node *result = parse_expression(lexer);

  if (lexer->current->type != TOKEN_EOF) {
    printf("Warning: Not all tokens were processed, stopped at position %zu\n",
           lexer->current_index);
  }

  free_lexer(lexer);
  return result;
}

ast_node *parse_expression(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse expression, lexer ptr is null");

  ast_node *left = parse_term(lexer);

  while (lexer->current->type == TOKEN_PLUS ||
         lexer->current->type == TOKEN_MINUS) {
    TokenType op = lexer->current->type;
    lexer_skip(lexer, 1);
    ast_node *right = parse_term(lexer);

    left = new_binary_node(left, right, op);
  }

  return left;
}

ast_node *parse_term(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse term by null ptr on lexer");

  ast_node *left = parse_factor(lexer);

  while (lexer->current->type == TOKEN_MULTIPLY ||
         lexer->current->type == TOKEN_DIVIDE) {
    TokenType op = lexer->current->type;
    lexer_skip(lexer, 1);
    ast_node *right = parse_factor(lexer);

    left = new_binary_node(left, right, op);
  }

  return left;
}

ast_node *parse_factor(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse factor by null ptr on lexer");

  if (lexer->current->type == TOKEN_NUMBER) {
    double value = lexer->current->value.number;
    lexer_skip(lexer, 1);
    return new_number_node(value);
  }

  if (lexer->current->type == TOKEN_LPAREN) {
    lexer_skip(lexer, 1);
    ast_node *expression = parse_expression(lexer);

    if (lexer->current->type != TOKEN_RPAREN)
      elog("Expected ')' at position %zu", lexer->current_index);

    lexer_skip(lexer, 1);
    return expression;
  }

  elog("Unexpected token: %d at position %zu", lexer->current->type,
       lexer->current_index);
  return NULL;
}
