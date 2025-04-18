#include "lexer.h"
#include "logger.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

ast_node *new_function_def_node(const char *name, arr_t *parameters,
                                ast_node *body) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (function definition)");

  node->type = NODE_FUNCTION_DEF;
  node->data.function_def.name = strdup(name);
  node->data.function_def.params = parameters;
  node->data.function_def.body = body;

  return node;
}

ast_node *new_function_call_node(const char *name, arr_t *arguments) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (function call)");

  node->type = NODE_FUNCTION_CALL;
  node->data.function_call.name = strdup(name);
  node->data.function_call.arguments = arguments;

  return node;
}

ast_node *new_return_node(ast_node *value) {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (return)");

  node->type = NODE_RETURN;
  node->data.return_stm.value = value;

  return node;
}

ast_node *new_loop_stop_node() {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (number type)");

  node->type = NODE_LOOP_STOP;
  return node;
}

ast_node *new_loop_next_node() {
  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (number type)");

  node->type = NODE_LOOP_NEXT;
  return node;
}

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

ast_node *new_variable_node(const char *name, bool is_const) {
  if (!name)
    elog("Can't create new variable ast node with null ptr on it name");
  if (*name == '\0')
    elog("Can't create new variable ast node with empty name");

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (variable type)");

  node->type = NODE_VARIABLE;
  node->data.var.var_name = strdup(name);
  node->data.var.is_const = is_const;

  return node;
}

ast_node *new_assignment_node(const char *var_name, ast_node *value,
                              bool is_const) {
  if (!var_name)
    elog("Can't create assigment ast node with null ptr on var name");
  if (*var_name == '\0')
    elog("Can't create assigment ast node with empty var name");
  if (!value)
    elog("Can't create assigments ast node with null ptr on value ast node");

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (assigment type)");

  node->type = NODE_ASSIGNMENT;
  node->data.assignment.var_name = strdup(var_name);
  node->data.assignment.value = value;
  node->data.assignment.is_const = is_const;

  return node;
}

ast_node *new_if_node(ast_node *condition, ast_node *if_body,
                      ast_node *else_body) {
  if (!condition)
    elog("Can't create if node with null ptr on condition node");
  if (!if_body)
    elog("Can't create if node without if_body ast nod");

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (if type)");

  node->type = NODE_IF;
  node->data.if_stmt.if_body = if_body;
  node->data.if_stmt.else_body = else_body;
  node->data.if_stmt.condition = condition;

  return node;
}

ast_node *new_print_node(ast_node *expression) {
  if (!expression)
    elog("Can't create print ast node with null ptr on expression ast node");

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (print type)");

  node->type = NODE_PRINT;
  node->data.print.expression = expression;
  return node;
}

ast_node *new_block_node(arr_t *statements) {
  if (!statements)
    elog("Can't create new block ast node , with null ptr on statements arr");
  if (statements->size == 0)
    elog("Can't create new block ast node with empty statements arr");
  if (!statements->data)
    elog("Can't create new block ast node with null ptr on arr_t -> items");

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (statemenets type)");

  node->type = NODE_BLOCK;
  node->data.block.statements = statements;
  return node;
}

ast_node *new_loop_node(ast_node *condition, ast_node *loop_body) {
  if (!condition)
    elog("Can't create loop ast node with null ptr on contidion ast node");
  if (!loop_body)
    elog("Can't create loop ast node with null ptr on loop body ast node");

  ast_node *node = (ast_node *)malloc(sizeof(ast_node));
  if (!node)
    elog("Error allocation memory for ast node (loop type)");

  node->type = NODE_LOOP;
  node->data.loop.condition = condition;
  node->data.loop.loop_body = loop_body;
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
  case NODE_ASSIGNMENT:
    free(node->data.assignment.var_name);
    free_ast(node->data.assignment.value);
    break;
  case NODE_VARIABLE:
    free(node->data.var.var_name);
    break;
  case NODE_IF:
    free_ast(node->data.if_stmt.condition);
    free_ast(node->data.if_stmt.if_body);
    if (node->data.if_stmt.else_body)
      free_ast(node->data.if_stmt.else_body);
    break;
  case NODE_PRINT:
    free_ast(node->data.print.expression);
    break;
  case NODE_BLOCK:
    for (size_t i = 0; i < node->data.block.statements->size; i++) {
      free_ast(arr_get(node->data.block.statements, i));
    }
    arr_destroy(node->data.block.statements);
    break;
  case NODE_LOOP:
    free_ast(node->data.loop.condition);
    free_ast(node->data.loop.loop_body);
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

void lexer_one_skip(lexer_t *lexer) {
  if (!lexer)
    elog("Can't skip one token at lexer , null ptr on it");

  lexer_skip(lexer, 1);
}

void lexer_syntax_error(lexer_t *lexer, const char *message) {
  if (!lexer)
    elog("Can't log syntax error , lexer ptr is null");
  if (!message || *message == '\0')
    elog("Can't log syntax error , message is null or empty string");

  elog("Syntax error %zu:%zu %s", lexer->current->line, lexer->current->offset,
       message);
}

bool lexer_skip_if_eq(lexer_t *lexer, TokenType type) {
  if (!lexer)
    elog("Can't skip if eq , lexer is null");

  if (lexer->current->type != type)
    elog("Unexpected token type , excpect %d , have : %d", type,
         lexer->current->type);

  lexer_one_skip(lexer);
  return true;
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
    case TOKEN_EQ:
      printf("==\n");
      break;
    case TOKEN_NE:
      printf("!=\n");
      break;
    case TOKEN_LT:
      printf("<\n");
      break;
    case TOKEN_LE:
      printf("<=\n");
      break;
    case TOKEN_GT:
      printf(">\n");
      break;
    case TOKEN_GE:
      printf(">=\n");
      break;
    default:
      printf("UNKNOWN (%d)\n", node->data.binary.op);
      break;
    }

    print_ast(node->data.binary.left, indent + 1);
    print_ast(node->data.binary.right, indent + 1);
    break;

  case NODE_VARIABLE:
    printf("VARIABLE: %s\n", node->data.var.var_name);
    break;

  case NODE_ASSIGNMENT:
    printf("ASSIGNMENT: %s =\n", node->data.assignment.var_name);
    print_ast(node->data.assignment.value, indent + 1);
    break;

  case NODE_IF:
    printf("IF:\n");
    printf("%*s  CONDITION:\n", indent * 2, "");
    print_ast(node->data.if_stmt.condition, indent + 2);
    printf("%*s  THEN:\n", indent * 2, "");
    print_ast(node->data.if_stmt.if_body, indent + 2);
    if (node->data.if_stmt.else_body) {
      printf("%*s  ELSE:\n", indent * 2, "");
      print_ast(node->data.if_stmt.else_body, indent + 2);
    }
    break;

  case NODE_PRINT:
    printf("PRINT:\n");
    print_ast(node->data.print.expression, indent + 1);
    break;

  case NODE_BLOCK:
    printf("BLOCK:\n");
    for (size_t i = 0; i < node->data.block.statements->size; i++) {
      print_ast(arr_get(node->data.block.statements, i), indent + 1);
    }
    break;

  case NODE_NOOP:
    printf("NO-OP\n");
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
  ast_node *result = NULL;

  if (lexer->current->type == TOKEN_LBRACE) {
    lexer_skip(lexer, 1);
    result = parse_block(lexer);
    if (lexer->current->type == TOKEN_RBRACE) {
      lexer_skip(lexer, 1);
    } else {
      lexer_syntax_error(lexer, "Expected '}'");
    }
  } else {
    arr_t *statements = arr_create(1);

    while (lexer->current->type != TOKEN_EOF) {
      ast_node *statement = parse_statement(lexer);
      if (statement) {
        arr_push(statements, statement);
      }
    }

    if (statements->size > 0) {
      result = new_block_node(statements);
    } else {
      arr_destroy(statements);
      elog("No valid statements found in script");
    }
  }

  free_lexer(lexer);
  return result;
}

ast_node *parse_block(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse block to ast node , lexer is null");

  arr_t *arr = arr_create(1);
  while (lexer->current->type != TOKEN_RBRACE &&
         lexer->current->type != TOKEN_EOF) {
    ast_node *statement = parse_statement(lexer);

    if (statement)
      arr_push(arr, statement);
  }

  if (arr->size == 0) {
    elog("Empty block statements are not allowed");
  }

  return new_block_node(arr);
}

ast_node *parse_statement(lexer_t *lexer) {
  if (!lexer)
    elog("Can't create statement ast node, have null ptr on lexer");

  if (lexer->current->type == TOKEN_SEMICOLON) {
    lexer_one_skip(lexer);
    return NULL; // Skip empty statements
  }

  if (lexer->current->type == TOKEN_LOOP_STOP) {
    lexer_one_skip(lexer);

    if (lexer->current->type != TOKEN_SEMICOLON)
      elog("Syntax error: %zu:%zu expected ';' after 'stop'",
           lexer->current->line, lexer->current->offset);

    lexer_one_skip(lexer);
    return new_loop_stop_node();
  }

  if (lexer->current->type == TOKEN_LOOP_NEXT) {
    lexer_one_skip(lexer);

    if (lexer->current->type != TOKEN_SEMICOLON)
      elog("Syntax error: %zu:%zu expected ';' after 'next'",
           lexer->current->line, lexer->current->offset);

    lexer_one_skip(lexer);
    return new_loop_next_node();
  }

  if (lexer->current->type == TOKEN_CONST) {

    lexer_one_skip(lexer);

    if (lexer->current->type != TOKEN_IDENTIFIER) {
      lexer_syntax_error(lexer, "after 'const' must go var identifire \n");
    }

    if (lexer->current->type == TOKEN_IDENTIFIER) {
      const char *var_name = lexer->current->value.string;
      lexer_one_skip(lexer);
      if (lexer->current->type != TOKEN_ASSIGN)
        elog("Syntax error : %zu:%zu expected '=' after identifier in "
             "assignment",
             lexer->current->line, lexer->current->offset);

      lexer_one_skip(lexer);
      ast_node *expression = parse_expression(lexer);

      if (lexer->current->type != TOKEN_SEMICOLON)
        elog("Syntax error : %zu:%zu expected ';' after expression",
             lexer->current->line, lexer->current->offset);

      lexer_one_skip(lexer);
      return new_assignment_node(var_name, expression, true);
    }
  }

  if (lexer->current->type == TOKEN_IDENTIFIER) {
    const char *var_name = lexer->current->value.string;
    lexer_one_skip(lexer);
    if (lexer->current->type != TOKEN_ASSIGN)
      elog("Syntax error : %zu:%zu expected '=' after identifier in assignment",
           lexer->current->line, lexer->current->offset);

    lexer_one_skip(lexer);
    ast_node *expression = parse_expression(lexer);

    if (lexer->current->type != TOKEN_SEMICOLON)
      elog("Syntax error : %zu:%zu expected ';' after expression",
           lexer->current->line, lexer->current->offset);

    lexer_one_skip(lexer);
    return new_assignment_node(var_name, expression, false);
  }

  if (lexer->current->type == TOKEN_FN) {
    return parse_function_def(lexer);
  }

  if (lexer->current->type == TOKEN_RETURN) {
    return parse_return_statement(lexer);
  }

  if (lexer->current->type == TOKEN_IF) {
    return parse_if_statement(lexer);
  }

  if (lexer->current->type == TOKEN_PRINT) {
    return parse_print_statement(lexer);
  }

  if (lexer->current->type == TOKEN_LOOP) {
    return parse_loop_statement(lexer);
  }

  if (lexer->current->type == TOKEN_LBRACE) {
    lexer_one_skip(lexer);
    ast_node *block = parse_block(lexer);
    if (lexer->current->type != TOKEN_RBRACE)
      elog("Syntax error : %zu:%zu expected '}' after block",
           lexer->current->line, lexer->current->offset);

    lexer_one_skip(lexer);
    return block;
  }

  lexer_syntax_error(lexer, "Unknown statement type");
  return NULL;
}

ast_node *parse_if_statement(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse if statement, lexer is null");

  lexer_skip_if_eq(lexer, TOKEN_IF);

  if (lexer->current->type != TOKEN_LPAREN)
    elog("Syntax error : %zu:%zu expected '(' after 'if'", lexer->current->line,
         lexer->current->offset);

  lexer_one_skip(lexer);
  ast_node *condition = parse_comparison(lexer);

  if (lexer->current->type != TOKEN_RPAREN)
    elog("Syntax error : %zu:%zu expected ')' after condition",
         lexer->current->line, lexer->current->offset);

  lexer_one_skip(lexer);
  ast_node *if_body = parse_statement(lexer);

  ast_node *else_body = NULL;
  if (lexer->current->type == TOKEN_ELSE) {
    lexer_one_skip(lexer);
    else_body = parse_statement(lexer);
  }

  return new_if_node(condition, if_body, else_body);
}

ast_node *parse_loop_statement(lexer_t *lexer) {
  if (lexer == NULL)
    elog("Can't parse loop with null ptr on lexer");

  lexer_skip_if_eq(lexer, TOKEN_LOOP);
  if (lexer->current->type != TOKEN_LPAREN)
    elog("Syntax error : %zu:%zu expected '(' after 'loop'",
         lexer->current->line, lexer->current->offset);

  lexer_skip_if_eq(lexer, TOKEN_LPAREN);
  ast_node *condition = parse_comparison(lexer);

  if (lexer->current->type != TOKEN_RPAREN)
    elog("Syntax error : %zu:%zu expected ')' after 'loop'",
         lexer->current->line, lexer->current->offset);
  lexer_skip_if_eq(lexer, TOKEN_RPAREN);

  ast_node *loop_body = parse_statement(lexer);

  return new_loop_node(condition, loop_body);
}

ast_node *parse_print_statement(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse print statement, lexer is null");

  lexer_skip_if_eq(lexer, TOKEN_PRINT);

  if (lexer->current->type != TOKEN_LPAREN)
    elog("Syntax error : %zu:%zu expected '(' after 'print'",
         lexer->current->line, lexer->current->offset);

  lexer_one_skip(lexer);
  ast_node *expression = parse_expression(lexer);

  if (lexer->current->type != TOKEN_RPAREN)
    elog("Syntax error : %zu:%zu expected ')' after expression in print",
         lexer->current->line, lexer->current->offset);

  lexer_one_skip(lexer);

  if (lexer->current->type != TOKEN_SEMICOLON)
    elog("Syntax error : %zu:%zu expected ';' after print statement",
         lexer->current->line, lexer->current->offset);

  lexer_one_skip(lexer);

  return new_print_node(expression);
}

ast_node *parse_function_def(lexer_t *lexer) {
  lexer_skip_if_eq(lexer, TOKEN_FN);

  if (lexer->current->type != TOKEN_IDENTIFIER)
    lexer_syntax_error(lexer, "after 'fn' must go identifier");

  char *name = strdup(lexer->current->value.string);
  lexer_one_skip(lexer);

  if (lexer->current->type != TOKEN_LPAREN)
    lexer_syntax_error(
        lexer,
        "after function identifier must go '(' params|or empty place ')' ");
  lexer_skip_if_eq(lexer, TOKEN_LPAREN);

  arr_t *params = arr_create(4);
  if (lexer->current->type != TOKEN_RPAREN) {
    do {
      if (lexer->current->type != TOKEN_IDENTIFIER)
        lexer_syntax_error(
            lexer, "expected params or ')' in function declaration after '(' ");

      char *param = strdup(lexer->current->value.string);
      arr_push(params, param);
      lexer_one_skip(lexer);

      if (lexer->current->type == TOKEN_COMMA)
        lexer_one_skip(lexer);
      else
        break;
    } while (true);
  }

  lexer_skip_if_eq(lexer, TOKEN_RPAREN);

  if (lexer->current->type == TOKEN_ARROW) {
    lexer_one_skip(lexer);
    ast_node *expr = parse_expression(lexer);

    ast_node *return_node = new_return_node(expr);

    arr_t *stms = arr_create(1);
    arr_push(stms, expr);
    ast_node *block_node = new_block_node(stms);

    if (lexer->current->type != TOKEN_SEMICOLON)
      lexer_syntax_error(lexer, "expected ';' after func expression");
    else
      lexer_one_skip(lexer);

    ast_node *func_def_node = new_function_def_node(name, params, block_node);
    return func_def_node;
  }

  if (lexer->current->type == TOKEN_LBRACE) {
    lexer_one_skip(lexer);

    ast_node *block = parse_block(lexer);

    if (lexer->current->type != TOKEN_RBRACE)
      lexer_syntax_error(lexer, "expected '}' in the on of func body");
    else
      lexer_one_skip(lexer);

    ast_node *func_def_node = new_function_def_node(name, params, block);
    return func_def_node;
  }

  lexer_syntax_error(lexer, "unexpected symbol after fynction "
                            "declaration , must be or '->' or '{' ");
}

ast_node *parse_function_call(lexer_t *lexer, const char *name) {
  if (!lexer)
    elog("Can't parse function call with null ptr on lexer");
  if (!name || *name == '\0')
    elog("Can't parse function call with null or empty function name");

  lexer_skip_if_eq(lexer, TOKEN_LPAREN);

  arr_t *arguments = arr_create(4);

  if (lexer->current->type != TOKEN_RPAREN) {
    do {
      if (lexer->current->type != TOKEN_IDENTIFIER)
        lexer_syntax_error(lexer, "expected list of parameters of ')' ");

      ast_node *arg_expr = parse_expression(lexer);
      arr_push(arguments, arg_expr);

      if (lexer->current->type == TOKEN_COMMA)
        lexer_one_skip(lexer);
      else
        break;
    } while (true);
  }

  lexer_skip_if_eq(lexer, TOKEN_RPAREN);

  return new_function_call_node(name, arguments);
}

ast_node *parse_return_statement(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse return stm , lexer ptr is null");

  lexer_skip_if_eq(lexer, TOKEN_RETURN);

  ast_node *value = NULL;

  if (lexer->current->type != TOKEN_SEMICOLON)
    value = parse_expression(lexer);
  else
    value = new_number_node(0.0);

  lexer_skip_if_eq(lexer, TOKEN_SEMICOLON);

  return new_return_node(value);
}

ast_node *parse_comparison(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse comparison, lexer is null");

  ast_node *left = parse_expression(lexer);

  if (lexer->current->type == TOKEN_EQ || lexer->current->type == TOKEN_NE ||
      lexer->current->type == TOKEN_LT || lexer->current->type == TOKEN_LE ||
      lexer->current->type == TOKEN_GT || lexer->current->type == TOKEN_GE) {

    TokenType op = lexer->current->type;
    lexer_one_skip(lexer);
    ast_node *right = parse_expression(lexer);

    return new_binary_node(left, right, op);
  }

  return left;
}

ast_node *parse_expression(lexer_t *lexer) {
  if (!lexer)
    elog("Can't parse expression, lexer ptr is null");

  ast_node *left = parse_term(lexer);

  while (lexer->current->type == TOKEN_PLUS ||
         lexer->current->type == TOKEN_MINUS) {
    TokenType op = lexer->current->type;
    lexer_one_skip(lexer);
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
    lexer_one_skip(lexer);
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
    lexer_one_skip(lexer);
    return new_number_node(value);
  }

  if (lexer->current->type == TOKEN_CONST) {
    if (lexer->current->type == TOKEN_IDENTIFIER) {
      lexer_syntax_error(lexer, "Can't create const var with no assigment");
    }
  }

  if (lexer->current->type == TOKEN_IDENTIFIER) {
    const char *name = lexer->current->value.string;
    lexer_one_skip(lexer);

    if (lexer->current->type == TOKEN_LPAREN)
      return parse_function_call(lexer, name);
    else
      return new_variable_node(name, false);
  }

  if (lexer->current->type == TOKEN_LPAREN) {
    lexer_one_skip(lexer);
    ast_node *expression = parse_expression(lexer);

    if (lexer->current->type != TOKEN_RPAREN)
      elog("Expected ')' at position %zu", lexer->current_index);

    lexer_one_skip(lexer);
    return expression;
  }

  elog("Unexpected token: %d at position %zu", lexer->current->type,
       lexer->current_index);
  return NULL;
}
