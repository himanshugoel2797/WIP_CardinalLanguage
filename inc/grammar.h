#ifndef CUSTINTER_GRAMMAR_H_
#define CUSTINTER_GRAMMAR_H_

#include <stdlib.h>

#define TKN_MAX_LEN 128

typedef enum {
  UNKNOWN,
  KEYWORD,
  ASSIGNMENT,
  DELIMITER,
  CHAR,
  STRING,
  IDENTIFIER,
  FLOAT,
  INTEGER,
  HEX,
  BINARY,
  BUILTIN_TYPE,
  BLOCK,
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
} Types;

typedef enum {
  CLASS,
  NAMESPACE,
  USING,
  STATIC,
  PUBLIC,
  PRIVATE,
  PROTECTED,
  INTERFACE,
  IF,
  ELSE,
  WHILE,
  DO,
  CASE,
  ENUM,
  STRUCT,
  CONST,
  BREAK,
  CONTINUE,
  RETURN,
  SIZEOF,
  NAMEOF,
  TYPEOF,
  VOLATILE,
  SWITCH,
  INTERNAL,
  THIS,
  IMPLICIT,
  OPERATOR,
  EXPLICIT,
  REF,
  IN,
  OUT,
  LOCK,
  TRY,
  CATCH,
  FINALLY,
  NEW,
  GET,
  SET,
  PARAMS,
  DELEGATE,
  ABSTRACT,
} Keywords;

static char *keyword_strs[] = {
    "class",     "namespace", "using",    "static",   "public",   "private",
    "protected", "interface", "if",       "else",     "while",    "do",
    "case",      "enum",      "struct",   "const",    "break",    "continue",
    "return",    "sizeof",    "nameof",   "typeof",   "volatile", "switch",
    "internal",  "this",      "implicit", "operator", "explicit", "ref",
    "in",        "out",       "lock",     "try",      "catch",    "finally",
    "new",       "get",       "set",      "params",   "delegate", "abstract",
    NULL};

static char *builtin_type_strs[] = {"int",   "uint",  "long",   "ulong", "byte",
                                    "sbyte", "short", "ushort", "float", NULL};

static char *operator_strs[] = {">",  "<",  ">=", "<=", "==", "!=", "!",
                                "&",  "|",  "^",  "||", "&&", "<<", ">>",
                                "++", "--", "+",  "-",  "*",  "/",  NULL};

#endif