#include "arr.h"
#include "parser.h"
#include <stdio.h>

const char *token_type_to_str(TokenType type) {
  switch (type) {
  case TOKEN_EOF:
    return "EOF";
  case TOKEN_NUMBER:
    return "NUMBER";
  case TOKEN_IDENTIFIER:
    return "IDENTIFIER";
  case TOKEN_ASSIGN:
    return "ASSIGN";
  case TOKEN_PLUS:
    return "PLUS";
  case TOKEN_MINUS:
    return "MINUS";
  case TOKEN_MULTIPLY:
    return "MULTIPLY";
  case TOKEN_DIVIDE:
    return "DIVIDE";
  case TOKEN_LPAREN:
    return "LPAREN";
  case TOKEN_RPAREN:
    return "RPAREN";
  case TOKEN_LBRACE:
    return "LBRACE";
  case TOKEN_RBRACE:
    return "RBRACE";
  case TOKEN_SEMICOLON:
    return "SEMICOLON";
  case TOKEN_IF:
    return "IF";
  case TOKEN_ELSE:
    return "ELSE";
  case TOKEN_PRINT:
    return "PRINT";
  case TOKEN_GT:
    return "GT";
  case TOKEN_LT:
    return "LT";
  case TOKEN_EQ:
    return "EQ";
  case TOKEN_LE:
    return "LE";
  case TOKEN_GE:
    return "GE";
  case TOKEN_NE:
    return "NE";
  default:
    return "UNKNOWN";
  }
}

int main(void) {
  char *code = "(5 + 2) * 3 - 4";
  arr_t *tokens = parse(code);

  printf("Parsed tokens for code: \"%s\"\n", code);
  printf("----------------------------------------------------\n");
  printf("| %-15s | %-15s | %-10s | %-10s |\n", "TYPE", "VALUE", "LINE",
         "OFFSET");
  printf("----------------------------------------------------\n");

  for (size_t i = 0; i < tokens->size; i++) {
    token *t = (token *)arr_get(tokens, i);

    if (t->type == TOKEN_NUMBER) {
      printf("| %-15s | %-15.2f | %-10zu | %-10zu |\n",
             token_type_to_str(t->type), t->value.number, t->line, t->offset);
    } else if (t->type == TOKEN_IDENTIFIER) {
      printf("| %-15s | %-15s | %-10zu | %-10zu |\n",
             token_type_to_str(t->type), t->value.string, t->line, t->offset);
    } else {
      printf(
          "| %-15s | %-15s | %-10zu | %-10zu |\n", token_type_to_str(t->type),
          (t->type == TOKEN_EOF) ? ""
                                 : ((t->value.string) ? t->value.string : ""),
          t->line, t->offset);
    }
  }
  printf("----------------------------------------------------\n");

  for (size_t i = 0; i < tokens->size; i++) {
    token *t = (token *)arr_get(tokens, i);

    if (t->type != TOKEN_NUMBER && t->type != TOKEN_EOF && t->value.string) {
      free(t->value.string);
    }

    free(t);
  }
  arr_destroy(tokens);
  return 0;
}
