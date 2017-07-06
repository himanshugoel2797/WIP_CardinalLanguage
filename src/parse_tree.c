#include "parse_tree.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static node_t *root;

void report_error(int code) {
  printf("ERROR: %d\r\n", code);
  exit(0);
}

void report_warning(char *msg) { printf("WARNING: %s\r\n", msg); }

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

void append_next(node_t *parent, node_t *val) {

  printf("Append Next \"%s\" to \"%s\"\r\n", val->token_val, parent->token_val);
  if (parent->next == NULL) {
    parent->next = val;
    return;
  }

  node_t *cursor = parent->next;
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
  tmp->extra_tags = 0;
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
  if (get_token_type() == OPERATOR && *get_token_val() == '.') {
    return create_curtoken_node();
  } else
    return NULL;
}

node_t *parse_name(void) {
  if (get_token_type() == IDENTIFIER) {
    node_t *p = create_curtoken_node();

    peek_token();
    node_t *c = parse_dot();

    if (c == NULL) {
      revert_peek_token();
    } else {
      append_child(p, c);

      apply_peek_token();
      next_token();

      node_t *d = parse_name();
      if (d != NULL)
        append_child(c, d);
    }

    return p;
  } else
    return NULL;
}

node_t *parse_code(void) {
  if (get_token_type() != BLOCK && *get_token_val() != '{')
    report_error(EXPECTED_OPENING_BRACE);

  return NULL;
}

node_t *parse_params(void) {

  if (get_token_type() != DELIMITER)
    return NULL;

  if (*get_token_val() != '(')
    return NULL;

  node_t *n = create_curtoken_node();
  n->extra_tags = PARAMETERS;

  bool expectedComma = false;
  bool expectedLast = false;

  while (true) {
    next_token();

    if (get_token_type() == IDENTIFIER && !expectedComma && !expectedLast) {
      node_t *p = create_curtoken_node();

      next_token();

      if (get_token_type() == KEYWORD && get_token_detail() == PARAMS) {
        node_t *varParam = create_curtoken_node();
        append_modifier(p, varParam);
        next_token();
        expectedLast = true;
      }

      if (get_token_type() != IDENTIFIER && get_token_type() != BUILTIN_TYPE)
        report_error(EXPECTED_TYPE);

      node_t *t = create_curtoken_node();
      append_modifier(p, t);
      append_child(n, p);
      expectedComma = true;
      continue;
    }

    if (get_token_type() == DELIMITER && *get_token_val() == ')')
      return n;

    if (expectedLast)
      report_error(VARARG_FINAL);

    if (expectedComma) {
      if (get_token_type() == DELIMITER && *get_token_val() == ',') {
        expectedComma = false;
        continue;
      }

      report_error(EXPECTED_COMMA);
    }
  }

  return NULL;
}

node_t *parse_func(void) {
  if (get_token_type() != CREATION)
    return NULL;

  if (get_token_detail() != FUNC)
    return NULL;

  node_t *isFunc = create_curtoken_node();

  next_token();
  if (get_token_type() != IDENTIFIER)
    report_error(EXPECTED_IDENTIFIER);

  node_t *ident = create_curtoken_node();
  append_modifier(ident, isFunc);

  next_token();
  node_t *params = parse_params();

  if (params == NULL)
    report_error(EXPECTED_PARAMETERS);

  append_child(ident, params);

  next_token();
  if (get_token_type() != IDENTIFIER && get_token_type() != BUILTIN_TYPE)
    report_error(EXPECTED_IDENTIFIER);

  node_t *retType = create_curtoken_node();
  retType->extra_tags = RETURN_TYPE;
  append_child(ident, retType);

  next_token();
  node_t *code = parse_code();
  if (code != NULL)
    append_child(ident, code);

  return ident;
}

node_t *parse_delegate(void) {
  if (get_token_type() != CREATION)
    return NULL;

  if (get_token_detail() != DELEGATE)
    return NULL;

  node_t *isDelegate = create_curtoken_node();

  next_token();
  if (get_token_type() != IDENTIFIER)
    report_error(EXPECTED_IDENTIFIER);

  node_t *ident = create_curtoken_node();
  append_modifier(ident, isDelegate);

  next_token();
  node_t *params = parse_params();

  if (params == NULL)
    report_error(EXPECTED_PARAMETERS);

  append_child(ident, params);

  next_token();
  if (get_token_type() != IDENTIFIER && get_token_type() != BUILTIN_TYPE)
    report_error(EXPECTED_IDENTIFIER);

  node_t *retType = create_curtoken_node();
  retType->extra_tags = RETURN_TYPE;
  append_child(ident, retType);

  next_token();
  node_t *semi = parse_semicolon();
  if (semi != NULL) {
    free(semi);
    return ident;
  }

  report_error(EXPECTED_SEMICOLON);
  return NULL;
}

node_t *parse_struct(void) {
  if (get_token_type() != CREATION)
    return NULL;

  if (get_token_detail() != STRUCT)
    return NULL;

  node_t *isStruct = create_curtoken_node();

  next_token();
  if (get_token_type() != IDENTIFIER)
    report_error(EXPECTED_IDENTIFIER);

  node_t *ident = create_curtoken_node();
  append_modifier(ident, isStruct);

  next_token();
  if (get_token_type() != BLOCK)
    report_error(EXPECTED_OPENING_BRACE);

  if (*get_token_val() != '{')
    report_error(EXPECTED_OPENING_BRACE);

  while (true) {
    next_token();

    node_t *member = parse_namespace_member();
    if (member != NULL) {
      append_child(ident, member);
      continue;
    }

    if (get_token_type() == BLOCK && *get_token_val() == '}')
      return ident;

    report_error(UNEXPECTED_TOKEN);
  }
}

node_t *parse_enum_member(void) {
  if (get_token_type() != IDENTIFIER)
    return NULL;

  node_t *ident = create_curtoken_node();

  next_token();
  if (get_token_type() == DELIMITER && *get_token_val() == ',')
    return ident;

  if (get_token_type() != ASSIGNMENT)
    report_error(UNEXPECTED_TOKEN);

  next_token();
  if (get_token_type() != INTEGER)
    report_error(UNEXPECTED_TOKEN);

  node_t *val = create_curtoken_node();
  append_child(ident, val);

  next_token();
  if (get_token_type() == DELIMITER && *get_token_val() == ',')
    return ident;

  report_error(EXPECTED_COMMA);
  return NULL;
}

node_t *parse_enum(void) {
  if (get_token_type() != CREATION | get_token_detail() != ENUM)
    return NULL;

  node_t *isEnum = create_curtoken_node();

  next_token();
  if (get_token_type() != IDENTIFIER)
    report_error(EXPECTED_IDENTIFIER);

  node_t *ident = create_curtoken_node();
  append_modifier(ident, isEnum);

  next_token();
  if (get_token_type() != BLOCK)
    report_error(EXPECTED_OPENING_BRACE);

  if (*get_token_val() != '{')
    report_error(EXPECTED_OPENING_BRACE);

  while (true) {
    next_token();
    node_t *member = parse_enum_member();
    if (member != NULL) {
      append_child(ident, member);
      continue;
    }

    if (get_token_type() == BLOCK && *get_token_val() == '}')
      return ident;

    report_error(UNEXPECTED_TOKEN);
  }

  return NULL;
}

node_t *parse_var(void) {
  if (get_token_type() != CREATION)
    return NULL;

  if (get_token_detail() != VAR)
    return NULL;

  node_t *isVar = create_curtoken_node();

  next_token();
  if (get_token_type() != IDENTIFIER && get_token_type() != BUILTIN_TYPE)
    report_error(EXPECTED_TYPE);

  node_t *type = create_curtoken_node();

  next_token();
  if (get_token_type() != IDENTIFIER)
    report_error(EXPECTED_IDENTIFIER);

  node_t *ident = create_curtoken_node();
  append_modifier(ident, isVar);
  append_modifier(ident, type);

  next_token();
  node_t *semi = parse_semicolon();
  if (semi != NULL) {
    free(semi);
    return ident; // Just a variable declaration.
  }

  // TODO: implement property support
  report_warning("Property support has not been implemented yet.");
  return ident;
}

node_t *parse_namespace_member(void) {

  if (get_token_type() != VISIBILITY_MODIFIER)
    return NULL;

  node_t *visibility_mod = create_curtoken_node();
  do {
    next_token();
    if (get_token_type() != MODIFIER)
      break;
    append_next(visibility_mod, create_curtoken_node());
  } while (true);

  if (get_token_type() != CREATION)
    report_error(EXPECTED_CREATION);

  node_t *p = NULL;
  switch (get_token_detail()) {
  case FUNC:
    p = parse_func();
    break;
  case DELEGATE:
    p = parse_delegate();
    break;
  case STRUCT:
    p = parse_struct();
    break;
  case ENUM:
    p = parse_enum();
    break;
  case VAR:
    p = parse_var();
    break;
  }

  if (p == NULL)
    report_error(EXPECTED_CREATION);

  append_modifier(p, visibility_mod);
  return p;
}

node_t *parse_namespace(void) {
  if (get_token_type() == CREATION && get_token_detail() == NAMESPACE) {
    node_t *p = create_curtoken_node();
    // parse namespace name

    next_token();
    node_t *c = parse_name();
    if (c == NULL) {
      // syntax error, missing identifier
      report_error(MISSING_IDENTIFIER);
    }
    append_child(p, c);

    next_token();
    if (get_token_type() != BLOCK)
      // syntax error, expected '{'
      report_error(EXPECTED_OPENING_BRACE);

    if (*get_token_val() != '{')
      report_error(EXPECTED_OPENING_BRACE);

    // parse namespace children
    while (true) {
      next_token();

      node_t *child = parse_namespace_member();
      if (child != NULL) {
        append_child(p, child);
        continue;
      }

      if (get_token_type() == BLOCK && *get_token_val() == '}')
        return p;

      // Execution should never get here unless an invalid token is present
      report_error(UNEXPECTED_TOKEN);
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
    if (get_token_type() == MODIFIER && get_token_detail() == CONST) {
      append_modifier(p, create_curtoken_node());
      next_token();
    }

    node_t *c = parse_name();
    if (c == NULL) {
      // syntax error, expected name
      report_error(MISSING_IDENTIFIER);
    }
    append_child(p, c);

    next_token();
    c = parse_semicolon();
    if (c == NULL) {
      // syntax error, expected semicolon
      report_error(EXPECTED_SEMICOLON);
    }

    free(c);
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

    if (get_token_type() == 0 && get_token_error() == END_FILE)
      break;

    // Execution should never get here unless an invalid token is present
    report_error(UNEXPECTED_TOKEN);
  }
}

node_t *generate_parsetree(void) {
  root = parse_global();
  return root;
}