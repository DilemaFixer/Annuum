#include "lexer.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для хранения переменных
typedef struct {
  char *name;
  double value;
} variable;

// Хранилище переменных (простая реализация)
typedef struct {
  variable *vars;
  size_t count;
  size_t capacity;
} variable_store;

// Инициализация хранилища переменных
variable_store *init_variable_store() {
  variable_store *store = malloc(sizeof(variable_store));
  store->vars = malloc(sizeof(variable) * 10);
  store->count = 0;
  store->capacity = 10;
  return store;
}

// Добавление или обновление переменной
void set_variable(variable_store *store, const char *name, double value) {
  // Поиск существующей переменной
  for (size_t i = 0; i < store->count; i++) {
    if (strcmp(store->vars[i].name, name) == 0) {
      store->vars[i].value = value;
      return;
    }
  }

  // Если переменная не найдена, добавляем новую
  if (store->count >= store->capacity) {
    store->capacity *= 2;
    store->vars = realloc(store->vars, sizeof(variable) * store->capacity);
  }

  store->vars[store->count].name = strdup(name);
  store->vars[store->count].value = value;
  store->count++;
}

// Получение значения переменной
double get_variable(variable_store *store, const char *name) {
  for (size_t i = 0; i < store->count; i++) {
    if (strcmp(store->vars[i].name, name) == 0) {
      return store->vars[i].value;
    }
  }
  elog("Variable '%s' not found", name);
  return 0.0;
}

// Освобождение памяти хранилища переменных
void free_variable_store(variable_store *store) {
  for (size_t i = 0; i < store->count; i++) {
    free(store->vars[i].name);
  }
  free(store->vars);
  free(store);
}

// Основная функция интерпретации
double interpret_with_vars(ast_node *ast_tree, variable_store *vars) {
  if (!ast_tree)
    elog("Can't interpret tree by null ptr");

  double one = 0;
  double two = 0;

  switch (ast_tree->type) {
  case NODE_NUMBER:
    return ast_tree->data.value;

  case NODE_VARIABLE:
    return get_variable(vars, ast_tree->data.var_name);

  case NODE_BIN_OP:
    one = interpret_with_vars(ast_tree->data.binary.left, vars);
    two = interpret_with_vars(ast_tree->data.binary.right, vars);

    switch (ast_tree->data.binary.op) {
    case TOKEN_PLUS:
      return one + two;
    case TOKEN_MINUS:
      return one - two;
    case TOKEN_MULTIPLY:
      return one * two;
    case TOKEN_DIVIDE:
      if (two == 0)
        elog("Can't divide by zero");
      return one / two;
    case TOKEN_GT:
      return one > two ? 1.0 : 0.0;
    case TOKEN_LT:
      return one < two ? 1.0 : 0.0;
    case TOKEN_EQ:
      return one == two ? 1.0 : 0.0;
    case TOKEN_GE:
      return one >= two ? 1.0 : 0.0;
    case TOKEN_LE:
      return one <= two ? 1.0 : 0.0;
    case TOKEN_NE:
      return one != two ? 1.0 : 0.0;
    default:
      elog("Unknown binary operator");
      return 0.0;
    }

  case NODE_ASSIGNMENT:
    one = interpret_with_vars(ast_tree->data.assignment.value, vars);
    set_variable(vars, ast_tree->data.assignment.var_name, one);
    return one;

  case NODE_IF:
    one = interpret_with_vars(ast_tree->data.if_stmt.condition, vars);
    if (one != 0.0) {
      return interpret_with_vars(ast_tree->data.if_stmt.if_body, vars);
    } else if (ast_tree->data.if_stmt.else_body) {
      return interpret_with_vars(ast_tree->data.if_stmt.else_body, vars);
    }
    return 0.0;

  case NODE_PRINT:
    one = interpret_with_vars(ast_tree->data.print.expression, vars);
    printf("%g\n", one);
    return one;

  case NODE_BLOCK:
    one = 0.0;
    for (size_t i = 0; i < ast_tree->data.block.statements->size; i++) {
      ast_node *statement = arr_get(ast_tree->data.block.statements, i);
      one = interpret_with_vars(statement, vars);
    }
    return one;

  case NODE_NOOP:
    return 0.0;

  default:
    elog("Unknown node type");
    return 0.0;
  }
}

// Обертка для совместимости
double interpret(ast_node *ast_tree) {
  variable_store *vars = init_variable_store();
  double result = interpret_with_vars(ast_tree, vars);
  free_variable_store(vars);
  return result;
}
