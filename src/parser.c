#include "parser.h"
#include "arr.h"
#include "logger.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

token *new_string_token(TokenType type, char *string, size_t line,
                        size_t offset) {
  if (!string)
    elog("Can't create a new string token with NULL ptr on string");
  if (*string == '\0')
    elog("Can't create a new string token with empty string");
  token *t = (token *)malloc(sizeof(token));
  if (!t)
    elog("Error allocation memory for token struct (string token)");
  t->type = type;
  t->line = line;
  t->offset = offset;
  t->value.string = string;
  return t;
}

token *new_number_token(TokenType type, double number, size_t line,
                        size_t offset) {
  token *t = (token *)malloc(sizeof(token));
  if (!t)
    elog("Error allocation memory for token struct (number token)");
  t->type = type;
  t->line = line;
  t->offset = offset;
  t->value.number = number;
  return t;
}

token *new_token(TokenType type, size_t line, size_t offset) {
  token *t = (token *)malloc(sizeof(token));
  if (!t)
    elog("Error allocation memory for token struct (simple token)");
  t->type = type;
  t->line = line;
  t->offset = offset;
  return t;
}

void skip(char **str, size_t count) {
  if (!str || !*str)
    elog("Can't skip char(s), ptr on string is NULL");
  size_t str_length = strlen(*str);
  if (count > str_length)
    elog("Can't skip more chars than %zu chars, string length is %zu", count,
         str_length);
  while (count != 0) {
    (*str)++;
    count--;
  }
}

char *cnext(char *c) {
  if (!c)
    elog("Can't check if have next, ptr on string is null");
  if (*(c + 1) == '\0')
    return NULL;
  return c + 1;
}

bool is_newline_character(char **c) {
  if (!c || !(*c))
    return false;
  if (*(*c) == '\r') {
    char *cn = cnext(*c);
    if (cn && *cn == '\n') {
      skip(c, 2);
    } else {
      skip(c, 1);
    }
    return true;
  }
  if (*(*c) == '\n') {
    skip(c, 1);
    return true;
  }
  return false;
}

bool is_system_symbol(char c) {
  return c == ')' || c == '(' || c == '{' || c == '}' || c == '+' || c == '-' ||
         c == '*' || c == '/' || c == '=' || c == ';' || c == '<' || c == '>' ||
         c == '!' || c == ',' || c == '.';
}

bool is_number(const char *str) {
  if (!str || !*str)
    return false;

  bool has_minus = (*str == '-');
  const char *p = str;

  if (has_minus)
    p++;

  if (!*p)
    return false;

  bool has_decimal = false;

  while (*p) {
    if (isdigit(*p)) {
      p++;
    } else if (*p == '.' && !has_decimal) {
      has_decimal = true;
      p++;
    } else {
      return false;
    }
  }
  return true;
}

bool is_keyword(const char *str, const char *keyword) {
  return strcmp(str, keyword) == 0;
}

TokenType get_token_type(const char *str) {
  if (!str || !*str)
    return TOKEN_EOF;

  if (is_number(str)) {
    return TOKEN_NUMBER;
  }

  if (is_keyword(str, "if"))
    return TOKEN_IF;
  if (is_keyword(str, "else"))
    return TOKEN_ELSE;
  if (is_keyword(str, "print"))
    return TOKEN_PRINT;
  if (is_keyword(str, "loop"))
    return TOKEN_LOOP;
  if (is_keyword(str, "next"))
    return TOKEN_LOOP_NEXT;
  if (is_keyword(str, "stop"))
    return TOKEN_LOOP_STOP;
  if (is_keyword(str, "const"))
    return TOKEN_CONST;
  if (is_keyword(str, "fn"))
    return TOKEN_FN;
  if (is_keyword(str, "return"))
    return TOKEN_RETURN;
  if (is_keyword(str, "->"))
    return TOKEN_ARROW;
  if (is_keyword(str, ","))
    return TOKEN_COMMA;

  if (is_keyword(str, "+"))
    return TOKEN_PLUS;
  if (is_keyword(str, "-"))
    return TOKEN_MINUS;
  if (is_keyword(str, "*"))
    return TOKEN_MULTIPLY;
  if (is_keyword(str, "/"))
    return TOKEN_DIVIDE;
  if (is_keyword(str, "="))
    return TOKEN_ASSIGN;
  if (is_keyword(str, "("))
    return TOKEN_LPAREN;
  if (is_keyword(str, ")"))
    return TOKEN_RPAREN;
  if (is_keyword(str, "{"))
    return TOKEN_LBRACE;
  if (is_keyword(str, "}"))
    return TOKEN_RBRACE;
  if (is_keyword(str, ";"))
    return TOKEN_SEMICOLON;
  if (is_keyword(str, ">"))
    return TOKEN_GT;
  if (is_keyword(str, "<"))
    return TOKEN_LT;

  if (is_keyword(str, "=="))
    return TOKEN_EQ;
  if (is_keyword(str, "<="))
    return TOKEN_LE;
  if (is_keyword(str, ">="))
    return TOKEN_GE;
  if (is_keyword(str, "!="))
    return TOKEN_NE;

  return TOKEN_IDENTIFIER;
}

char *extract_word(const char *str, size_t *length) {
  if (!str)
    return NULL;
  *length = 0;
  bool is_ssymbol = is_system_symbol(*str);
  bool potential_negative_number =
      (*str == '-' && (isdigit(*(str + 1)) || *(str + 1) == '.'));
  bool potential_decimal_point = (*str == '.' && isdigit(*(str + 1)));

  if (is_ssymbol && !potential_negative_number && !potential_decimal_point) {
    if ((*str == '=' && *(str + 1) == '=') ||
        (*str == '<' && *(str + 1) == '=') ||
        (*str == '>' && *(str + 1) == '=') ||
        (*str == '!' && *(str + 1) == '=') ||
        (*str == '-' && *(str + 1) == '>')) {
      *length = 2;
    } else {
      *length = 1;
    }
  } else {
    bool is_potential_number =
        isdigit(*str) || potential_negative_number || potential_decimal_point;
    while (str[*length] && !isspace(str[*length])) {
      if (is_system_symbol(str[*length])) {
        if (is_potential_number && str[*length] == '.' && str[*length + 1] &&
            isdigit(str[*length + 1])) {
          (*length)++;
          continue;
        }
        if (*length == 0 && potential_negative_number) {
          (*length)++;
          continue;
        }
        break;
      }
      (*length)++;
    }
  }

  char *result = (char *)malloc(*length + 1);
  if (!result) {
    return NULL;
  }
  for (size_t i = 0; i < *length; i++) {
    result[i] = str[i];
  }
  result[*length] = '\0';
  return result;
}

void handle_comment(char **code, size_t *line) {
  if (**code == '/' && *(*code + 1) == '/') {
    while (**code && **code != '\n' && **code != '\r') {
      (*code)++;
    }

    if (**code == '\n' || **code == '\r') {
      is_newline_character(code);
      (*line)++;
    }
  }
}

arr_t *parse(char *code) {
  if (!code || *code == '\0')
    elog("Can't parse empty code file");

  arr_t *tokens = arr_create(1);
  size_t line = 1;
  size_t offset = 0;

  while (*code != '\0') {
    if (isspace(*code)) {
      if (is_newline_character(&code)) {
        line++;
        offset = 0;
      } else {
        code++;
        offset++;
      }
      continue;
    }

    if (*code == '/' && *(code + 1) == '/') {
      handle_comment(&code, &line);
      continue;
    }

    size_t len = 0;
    char *stoken = extract_word(code, &len);

    if (!stoken || len == 0) {
      code++;
      offset++;
      continue;
    }

    TokenType type = get_token_type(stoken);
    token *t;

    if (type == TOKEN_NUMBER) {
      double value = atof(stoken);
      free(stoken);
      t = new_number_token(type, value, line, offset);
    } else {
      t = new_string_token(type, stoken, line, offset);
    }

    arr_push(tokens, t);
    skip(&code, len);
    offset += len;
  }

  token *eof_token = new_token(TOKEN_EOF, line, offset);
  arr_push(tokens, eof_token);

  return tokens;
}
