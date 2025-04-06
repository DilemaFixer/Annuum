#include "lexer.h"
#include "logger.h"

double interpret(ast_node *ast_tree) {
  if (!ast_tree)
    elog("Can't iterpret tree by null ptr");
  double one = 0;
  double two = 0;
  switch (ast_tree->type) {
  case NODE_NUMBER:
    return ast_tree->data.value;
    break;
  case NODE_BIN_OP:

    one = interpret(ast_tree->data.binary.left);
    two = interpret(ast_tree->data.binary.right);

    switch (ast_tree->data.binary.op) {
    case TOKEN_PLUS:
      return one + two;
      break;
    case TOKEN_MINUS:
      return one - two;
      break;
    case TOKEN_MULTIPLY:
      return one * two;
      break;
    case TOKEN_DIVIDE:
      if (one == 0)
        elog("Can't divede on zero");

      return one / two;
    }

    break;
  }
}
