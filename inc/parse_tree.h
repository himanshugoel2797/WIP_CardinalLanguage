#ifndef CUSTINTER_PARSETREE_H_
#define CUSTINTER_PARSETREE_H_

#include "grammar.h"

typedef enum { RETURN_TYPE, PARAMETERS, VARPARAMS } ExtraTags;

typedef struct node {
  int token_type;
  int token_detail;
  int extra_tags;
  char token_val[TKN_MAX_LEN];
  struct node *modifiers;
  struct node *children;
  struct node *next;
} node_t;

typedef enum {
  MISSING_IDENTIFIER,
  EXPECTED_OPENING_BRACE,
  EXPECTED_CLOSING_BRACE,
  EXPECTED_SEMICOLON,
  UNEXPECTED_TOKEN,
  EXPECTED_CREATION,
  EXPECTED_TYPE,
  EXPECTED_IDENTIFIER,
  EXPECTED_COMMA,
  VARARG_FINAL,
  EXPECTED_PARAMETERS,
} ParseTreeErrors;

node_t *generate_parsetree(void);

void report_error(int code);

node_t *parse_namespace_member(void);

#endif