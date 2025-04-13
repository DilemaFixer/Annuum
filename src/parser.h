#ifndef PARSER_H
#define PARSER_H
#include "arr.h"
#include <stdbool.h>

typedef enum {
    TOKEN_EOF = 0,     // Конец файла
    TOKEN_NUMBER,      // Число (123, 42.5)
    TOKEN_IDENTIFIER,  // Идентификатор (переменная)
    TOKEN_ASSIGN,      // Присваивание (=)
    TOKEN_PLUS,        // Сложение (+)
    TOKEN_MINUS,       // Вычитание (-)
    TOKEN_MULTIPLY,    // Умножение (*)
    TOKEN_DIVIDE,      // Деление (/)
    TOKEN_LPAREN,      // Левая скобка (
    TOKEN_RPAREN,      // Правая скобка )
    TOKEN_LBRACE,      // Левая фигурная скобка {
    TOKEN_RBRACE,      // Правая фигурная скобка }
    TOKEN_SEMICOLON,   // Точка с запятой (;)
    TOKEN_IF,          // Ключевое слово if
    TOKEN_ELSE,        // Ключевое слово else
    TOKEN_LOOP,        // Ключевое слово loop
    TOKEN_LOOP_NEXT,   // Ключевое слово next
    TOKEN_LOOP_STOP,   // Ключевое слово stop
    TOKEN_PRINT,       // Ключевое слово print
    TOKEN_GT,          // Больше (>)
    TOKEN_LT,          // Меньше (<)
    TOKEN_EQ,          // Равно (==)
    TOKEN_LE,          // Меньше или равно (<=)
    TOKEN_GE,          // Больше или равно (>=)
    TOKEN_NE,          // Не равно (!=)
} TokenType;

typedef struct token {
    TokenType type;
    size_t line;
    size_t offset;
    union {
        double number;
        char* string;
    } value;
} token;

token *new_string_token(TokenType type, char* string, size_t line, size_t offset);
token *new_number_token(TokenType type, double number, size_t line, size_t offset);
token *new_token(TokenType type, size_t line , size_t offset);

arr_t *parse(char* code);
void skip(char **str, size_t count);
char *cnext(char *c);
bool is_newline_character(char **c);
bool is_system_symbol(char c);
char *extract_word(const char *str, size_t *length);
bool is_number(const char *str);
bool is_keyword(const char *str, const char *keyword);
TokenType get_token_type(const char *str);
void handle_comment(char **code, size_t *line);

#endif // PARSER_H
