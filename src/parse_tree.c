#include "parse_tree.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>

static node_t *root;

void append_child(node_t *parent, node_t *val) {

  if (parent->children == NULL) {
    parent->children = val;
    return;
  }

  node_t *cursor = parent->children;
  while (cursor->next != NULL)
    cursor = cursor->next;

  cursor->next = val;
}

node_t *create_node(int type, int detail, const char *val) {
  node_t *tmp = malloc(sizeof(node_t));
  memset(tmp, 0, sizeof(node_t));

  tmp->token_type = type;
  tmp->token_detail = detail;
  if (val != NULL)
    strcpy(tmp->token_val, val);
  tmp->next = NULL;
  tmp->children = NULL;

  return tmp;
}

node_t *create_curtoken_node(void) {
  if (get_token_error() != NONE) {
    printf("ERROR: %d\r\n", get_token_error());
    exit(0);
  }
  return create_node(get_token_type(), get_token_detail(), get_token_val());
}

node_t *parse_enum(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == ENUM) {
    // TODO: parse name
  } else
    return NULL;
}

node_t *parse_struct(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == STRUCT) {
    // TODO: parse name
  } else
    return NULL;
}

node_t *parse_class(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == CLASS) {
    // TODO: parse name
  } else
    return NULL;
}

node_t *parse_namespace(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == NAMESPACE) {
    node_t *p = create_curtoken_node();
    // TODO: parse namespace name

    // parse namespace children
    while (true) {
      node_t *child;

      next_token();

      child = parse_enum();
      if (child != NULL) {
        append_child(p, child);
        continue;
      }

      child = parse_struct();
      if (child != NULL) {
        append_child(p, child);
        continue;
      }

      child = parse_class();
      if (child != NULL) {
        append_child(p, child);
        continue;
      }

      // TODO: Execution should never get here unless an invalid token is
      // present
    }
    return p;
  } else {
    return NULL;
  }
}

node_t *parse_import(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == IMPORT) {
    node_t *p = create_curtoken_node();

    next_token();

    // TODO: parse imported name

    return p;
  } else {
    return NULL;
  }
}

node_t *parse_global(void) {

  node_t *global = create_node(0, 0, NULL);

  while (true) {
    node_t *child;

    next_token();

    child = parse_import();
    if (child != NULL) {
      append_child(global, child);
      continue;
    }

    child = parse_namespace();
    if (child != NULL) {
      append_child(global, child);
      continue;
    }

    // TODO: Execution should never get here unless an invalid token is present
  }
}

void generate_parsetree(void) {
  root = parse_global();
  /*
    while (true) {
      int type = next_token();
      printf("%s : %d\r\n", get_token_val(), type);

      if (get_token_error() != NONE) {
        printf("ERROR: %d\r\n", get_token_error());
        break;
      }
    }*/
}