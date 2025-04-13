#include "lexer.h"
#include "logger.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOOP_NEXT_SIGNAL -123456789.0
#define LOOP_STOP_SIGNAL -987654321.0

typedef struct {
  char *name;
  arr_t *parameters;
  ast_node *body;
} function_definition;

typedef struct {
  function_definition *funcs;
  size_t count;
  size_t capacity;
} function_store;

typedef struct {
  char *name;
  double value;
  bool is_const;
} variable;

typedef struct {
  variable *vars;
  size_t count;
  size_t capacity;
} variable_store;

variable_store *init_variable_store() {
  variable_store *store = malloc(sizeof(variable_store));
  store->vars = malloc(sizeof(variable) * 10);
  store->count = 0;
  store->capacity = 10;
  return store;
}

void set_variable(variable_store *store, const char *name, double value,
                  bool is_const) {
  for (size_t i = 0; i < store->count; i++) {
    if (strcmp(store->vars[i].name, name) == 0) {
      if (store->vars[i].is_const) {
        elog("syntax error , try set value to const var %s", name);
      }

      store->vars[i].value = value;
      return;
    }
  }

  if (store->count >= store->capacity) {
    store->capacity *= 2;
    store->vars = realloc(store->vars, sizeof(variable) * store->capacity);
  }

  store->vars[store->count].name = strdup(name);
  store->vars[store->count].value = value;
  store->vars[store->count].is_const = is_const;
  store->count++;
}

double get_variable(variable_store *store, const char *name) {
  for (size_t i = 0; i < store->count; i++) {
    if (strcmp(store->vars[i].name, name) == 0) {
      return store->vars[i].value;
    }
  }
  elog("Variable '%s' not found", name);
  return 0.0;
}

void free_variable_store(variable_store *store) {
  for (size_t i = 0; i < store->count; i++) {
    free(store->vars[i].name);
  }
  free(store->vars);
  free(store);
}

function_store *init_function_store() {
  function_store *store = malloc(sizeof(function_store));
  store->funcs = malloc(sizeof(function_definition) * 10);
  store->count = 0;
  store->capacity = 10;
  return store;
}

void add_function(function_store *store, const char *name, arr_t *parameters,
                  ast_node *body) {
  for (size_t i = 0; i < store->count; i++) {
    if (strcmp(store->funcs[i].name, name) == 0) {
      elog("Function '%s' already defined", name);
    }
  }

  if (store->count >= store->capacity) {
    store->capacity *= 2;
    store->funcs =
        realloc(store->funcs, sizeof(function_definition) * store->capacity);
  }

  store->funcs[store->count].name = strdup(name);
  store->funcs[store->count].parameters = parameters;
  store->funcs[store->count].body = body;
  store->count++;
}

function_definition *get_function(function_store *store, const char *name) {
  for (size_t i = 0; i < store->count; i++) {
    if (strcmp(store->funcs[i].name, name) == 0) {
      return &store->funcs[i];
    }
  }
  elog("Function '%s' not found", name);
  return NULL;
}

double interpret_with_vars(ast_node *ast_tree, variable_store *vars,
                           function_store *funcs) {
  if (!ast_tree)
    elog("Can't interpret tree by null ptr");

  double one = 0;
  double two = 0;

  switch (ast_tree->type) {
  case NODE_NUMBER:
    return ast_tree->data.value;

  case NODE_LOOP_STOP:
    return LOOP_STOP_SIGNAL;

  case NODE_LOOP_NEXT:
    return LOOP_NEXT_SIGNAL;

  case NODE_VARIABLE:
    return get_variable(vars, ast_tree->data.var.var_name);

  case NODE_BIN_OP:
    one = interpret_with_vars(ast_tree->data.binary.left, vars, funcs);
    two = interpret_with_vars(ast_tree->data.binary.right, vars, funcs);

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
    one = interpret_with_vars(ast_tree->data.assignment.value, vars, funcs);
    set_variable(vars, ast_tree->data.assignment.var_name, one,
                 ast_tree->data.assignment.is_const);
    return one;

  case NODE_IF:
    one = interpret_with_vars(ast_tree->data.if_stmt.condition, vars, funcs);
    if (one != 0.0) {
      return interpret_with_vars(ast_tree->data.if_stmt.if_body, vars, funcs);
    } else if (ast_tree->data.if_stmt.else_body) {
      return interpret_with_vars(ast_tree->data.if_stmt.else_body, vars, funcs);
    }
    return 0.0;

  case NODE_LOOP:

    while (true) {
      one = interpret_with_vars(ast_tree->data.loop.condition, vars, funcs);

      if (one == 0.0)
        break;

      double result =
          interpret_with_vars(ast_tree->data.loop.loop_body, vars, funcs);

      if (result == LOOP_NEXT_SIGNAL)
        continue;

      if (result == LOOP_STOP_SIGNAL)
        break;
    }
    return 0.0;

  case NODE_PRINT:
    one = interpret_with_vars(ast_tree->data.print.expression, vars, funcs);
    printf("%g\n", one);
    return one;

  case NODE_BLOCK:
    one = 0.0;
    for (size_t i = 0; i < ast_tree->data.block.statements->size; i++) {
      ast_node *statement = arr_get(ast_tree->data.block.statements, i);
      one = interpret_with_vars(statement, vars, funcs);

      if (one == LOOP_NEXT_SIGNAL || one == LOOP_STOP_SIGNAL) {
        return one;
      }
    }
    return one;

  case NODE_FUNCTION_DEF:
    add_function(funcs, ast_tree->data.function_def.name,
                 ast_tree->data.function_def.params,
                 ast_tree->data.function_def.body);
    return 0.0;

  case NODE_FUNCTION_CALL: {
    function_definition *func =
        get_function(funcs, ast_tree->data.function_call.name);

    variable_store *local_vars = init_variable_store();

    size_t param_count = func->parameters->size;
    size_t arg_count = ast_tree->data.function_call.arguments->size;

    if (param_count != arg_count)
      elog("Function '%s' called with wrong number of arguments",
           ast_tree->data.function_call.name);

    for (size_t i = 0; i < param_count; i++) {
      char *param_name = (char *)arr_get(func->parameters, i);
      ast_node *arg_expr = arr_get(ast_tree->data.function_call.arguments, i);

      double arg_value = interpret_with_vars(arg_expr, vars, funcs);

      set_variable(local_vars, param_name, arg_value, false);
    }

    double result = interpret_with_vars(func->body, local_vars, funcs);

    free_variable_store(local_vars);

    return result;
  }

  case NODE_RETURN:
    return interpret_with_vars(ast_tree->data.return_stm.value, vars, funcs);

  case NODE_NOOP:
    return 0.0;

  default:
    elog("Unknown node type");
    return 0.0;
  }
}

double interpret(ast_node *ast_tree) {
  variable_store *vars = init_variable_store();
  function_store *funcs = init_function_store();
  double result = interpret_with_vars(ast_tree, vars, funcs);
  free_variable_store(vars);
  return result;
}
