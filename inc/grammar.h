#ifndef CUSTINTER_GRAMMAR_H_
#define CUSTINTER_GRAMMAR_H_

#include <stdlib.h>

#define TKN_MAX_LEN 128

typedef enum {
  UNKNOWN,
  KEYWORD,
  ASSIGNMENT,
  DELIMITER,
  OPERATOR,
  CHAR,
  STRING,
  IDENTIFIER,
  FLOAT,
  INTEGER,
  HEX,
  BINARY,
  BUILTIN_TYPE,
  BLOCK,
  VISIBILITY_MODIFIER,
  MODIFIER,
  CREATION,
} TokenTypes;

typedef enum {
  NONE,
  SYNTAX,
  PARENS,
  END_FILE,
  INVALID_SEQUENCE,
  UNEXPECTED_END,
  EXPECTED_TERMINATOR
} TokenErrors;

typedef enum {
  GT,
  LT,
  GE,
  LE,
  EQ,
  NE,
  NOT,
  AND,
  OR,
  XOR,
  LOR,
  LAND,
  LSHIFT,
  RSHIFT,
  INC,
  DEC,
  ADD,
  SUB,
  MULT,
  DIV,
  DOT,
  DEREF,
  MOD,
} Operators;

typedef enum {
  SINT32,
  UINT32,
  SINT64,
  UINT64,
  UINT8,
  SINT8,
  SINT16,
  UINT16,
  FLOAT32,
  FLOAT64,
  VOID,
  BOOL,
} Types;

typedef enum {
  PUBLIC,
  PRIVATE,
  INTERNAL,
} VisibilityModifiers;

typedef enum {
  ATOMIC,
  PTR,
  VOLATILE,
  CONST,
} Modifiers;

typedef enum {
  NAMESPACE,
  FUNC,
  DELEGATE,
  STRUCT,
  ENUM,
  VAR,
} Creations;

typedef enum {
  IF,
  ELSE,
  WHILE,
  DO,
  CASE,
  BREAK,
  CONTINUE,
  RETURN,
  SIZEOF,
  NAMEOF,
  SWITCH,
  GET,
  SET,
  PARAMS,
  ADDROF,
  VAL_AT,
  NULL_VAL,
  IMPORT,
  TRUE_VAL,
  FALSE_VAL,
  VALUE,
  FOR,
  DEFAULT,
} Keywords;

static char *visibility_modifier_strs[] = {"public", "private", "internal",
                                           NULL};

static char *modifier_strs[] = {"atomic", "ptr", "volatile", "const", NULL};

static char *creation_strs[] = {"namespace", "func", "delegate", "struct",
                                "enum",      "var",  NULL};

static char *keyword_strs[] = {
    "if",       "else",   "while",  "do",     "case",    "break",
    "continue", "return", "sizeof", "nameof", "switch",  "get",
    "set",      "params", "addrof", "val_at", "null",    "import",
    "true",     "false",  "value",  "for",    "default", NULL};

static char *builtin_type_strs[] = {
    "int",    "uint",  "long",   "ulong", "byte", "sbyte", "short",
    "ushort", "float", "double", "void",  "bool", NULL};

static char *operator_strs[] = {">", "<", ">=", "<=", "==", "!=", "!",  "&",
                                "|", "^", "||", "&&", "<<", ">>", "++", "--",
                                "+", "-", "*",  "/",  ".",  "->", "%",  NULL};

#endif