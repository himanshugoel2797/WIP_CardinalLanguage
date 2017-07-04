#ifndef CUSTINTER_PARSETREE_H_
#define CUSTINTER_PARSETREE_H_

#include "grammar.h"

typedef struct node {
  int token_type;
  int token_detail;
  char token_val[TKN_MAX_LEN];
  struct node *children;
  struct node *next;
} node_t;

#endif