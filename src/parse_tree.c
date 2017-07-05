#include "parse_tree.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>

static node_t *root;

void append_modifier(node_t *parent, node_t *val) {

  printf("Append Modifier \"%s\" to \"%s\"\r\n", val->token_val,
         parent->token_val);
  if (parent->modifiers == NULL) {
    parent->modifiers = val;
    return;
  }

  node_t *cursor = parent->modifiers;
  while (cursor->next != NULL)
    cursor = cursor->next;

  cursor->next = val;
}

void append_child(node_t *parent, node_t *val) {

  printf("Append Child \"%s\" to \"%s\"\r\n", val->token_val,
         parent->token_val);
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

  printf("Create Node: %s\r\n", val);

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

node_t *parse_semicolon(void) {
  if (get_token_type() == DELIMITER && *get_token_val() == ';') {
    return create_curtoken_node();
  } else
    return NULL;
}

node_t *parse_dot(void) {
  if (get_token_type() == DELIMITER && *get_token_val() == '.') {
    return create_curtoken_node();
  } else
    return NULL;
}

node_t *parse_name(void) {
  if (get_token_type() == IDENTIFIER) {
    node_t *p = create_curtoken_node();
    return p;
  } else
    return NULL;
}

node_t *parse_enum(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == ENUM) {
    node_t *p = create_curtoken_node();
    next_token();

    node_t *c = parse_name();
    if (c == NULL) {
      // TODO: syntax error, missing identifier
    }

    append_child(p, c);
    return p;
  } else
    return NULL;
}

node_t *parse_member(void) { return NULL; }

node_t *parse_struct(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == STRUCT) {
    node_t *p = create_curtoken_node();
    next_token();

    node_t *c = parse_name();
    if (c == NULL) {
      // TODO: syntax error, missing identifier
    }

    append_child(p, c);
    if (get_token_type() != BLOCK && *get_token_val() != '{') {
      // TODO: syntax error, expected '{'
    }

    // Add members
    while (true) {
      node_t *c = NULL;

      next_token();

      c = parse_member();
      if (c != NULL) {
        append_child(p, c);
        continue;
      }

      if (get_token_type() == BLOCK && *get_token_val() == '}')
        return p;
    }

    return p;
  } else
    return NULL;
}

node_t *parse_class(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == CLASS) {
    node_t *p = create_curtoken_node();
    next_token();

    node_t *c = parse_name();
    if (c == NULL) {
      // TODO: syntax error, missing identifier
    }
    append_child(p, c);

    if (get_token_type() != BLOCK && *get_token_val() != '{') {
      // TODO: syntax error, expected '{'
    }

    // Add members
    while (true) {
      node_t *c = NULL;

      next_token();

      c = parse_member();
      if (c != NULL) {
        append_child(p, c);
        continue;
      }

      if (get_token_type() == BLOCK && *get_token_val() == '}')
        return p;
    }

    return p;
  } else
    return NULL;
}

node_t *parse_typeDeclaration(void) {
  node_t *child = NULL;
  node_t *modifier = NULL;
  if (get_token_type() == KEYWORD) {
    switch (get_token_detail()) {
    case PUBLIC:
    case PRIVATE:
    case PROTECTED:
    case INTERNAL:
      modifier = create_curtoken_node();
      next_token();
      break;
    case CLASS:
    case STRUCT:
    case ENUM:
      modifier = create_node(KEYWORD, PRIVATE, "private");
      break;
    default:
      return NULL;
      break;
    }
  } else
    return NULL;

  child = parse_enum();
  if (child != NULL) {
    append_modifier(child, modifier);
    return child;
  }

  child = parse_struct();
  if (child != NULL) {
    append_modifier(child, modifier);
    return child;
  }

  child = parse_class();
  if (child != NULL) {
    append_modifier(child, modifier);
    return child;
  }

  // TODO: Execution should never get here unless an invalid token is
  // present
}

node_t *parse_namespace(void) {
  if (get_token_type() == KEYWORD && get_token_detail() == NAMESPACE) {
    node_t *p = create_curtoken_node();
    // TODO: parse namespace name

    next_token();
    node_t *c = parse_name();
    if (c == NULL) {
      // TODO: syntax error, missing identifier
    }
    append_child(p, c);

    next_token();
    if (get_token_type() != BLOCK && *get_token_val() != '{') {
      // TODO: syntax error, expected '{'
    }

    // parse namespace children
    while (true) {
      next_token();

      if (get_token_type() == BLOCK && *get_token_val() == '}')
        return p;

      node_t *child = parse_typeDeclaration();
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

    bool was_name = false;
    while (true) {
      node_t *c;

      next_token();

      c = parse_name();
      if (c != NULL) {
        if (was_name) {
          // TODO: syntax error, invalid name
        }

        append_child(p, c);
        was_name = true;
        continue;
      }

      c = parse_dot();
      if (c != NULL) {
        if (!was_name) {
          // TODO: syntax error, invalid name
        }

        append_child(p, c);
        was_name = false;
        continue;
      }

      c = parse_semicolon();
      if (c != NULL) {
        free(c);
        return p;
      }

      // TODO: syntax error, expected identifier
    }

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

    if (get_token_type() == 0)
      break;

    // TODO: Execution should never get here unless an invalid token is present
  }
}

node_t *generate_parsetree(void) {
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
  return root;
}