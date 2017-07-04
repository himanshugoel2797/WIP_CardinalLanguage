#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"
#include "tokenizer.h"

static const char *whitespace = " \t\r\n\v\f";
static const char *integersVals = "0123456789";
static const char *floatsVals = "0123456789.";
static const char *binaryVals = "01";
static const char *hexVals = "0123456789abcdefABCDEF";
static const char *identVals =
    "_$1234567890abcdefghijklmnopqrstuvwxyzABCEDFGHIJKLMNOPQRSTUVWXYZ";

static int tokenizer_data_len;
static char *tokenizer_data = NULL;
static char *token_pos;
static char token_val[TKN_MAX_LEN];
static int token_type;
static int token_detail;
static int token_error;

static bool inString = false;
static bool isEscape = false;

int get_token_type(void) { return token_type; }

int get_token_error(void) { return token_error; }

const char *get_token_val(void) { return token_val; }

void set_tokenizer_data(const char *data, int len) {
  if (tokenizer_data != NULL)
    free(tokenizer_data);
  tokenizer_data = malloc(len + 1);
  token_pos = tokenizer_data;
  tokenizer_data_len = len;

  memcpy(tokenizer_data, data, len + 1);
}

bool check_whitespace(char c) {

  if (c == 0)
    return true;

  for (int i = 0; i < 6; i++)
    if (c == whitespace[i])
      return true;
  return false;
}

bool has_whitespace(void) { return check_whitespace(*token_pos); }

int next_token(void) {
  token_type = UNKNOWN;
  memset(token_val, 0, TKN_MAX_LEN);

  if (*token_pos == 0) {
    token_error = END_FILE;
    return 0;
  }

  token_error = NONE;

  // Detect and remove whitespace and comments
  if (!inString) {
    bool inLineComment = false;
    bool inBlockComment = false;
    while (true) {
      if (*token_pos == 0) {
        token_error = NONE;
        return token_type;
      }

      if (inLineComment && !inBlockComment && *token_pos == '\n')
        inLineComment = false;

      if (inBlockComment && !inLineComment && *(token_pos - 1) == '*' &&
          *token_pos == '/')
        inBlockComment = false;

      // Also Parse and remove comments, treating them as whitespace
      if (!inLineComment && !inBlockComment && *token_pos == '/' &&
          *(token_pos + 1) == '/') {
        token_pos++;
        inLineComment = true;
      }

      if (!inLineComment && !inBlockComment && *token_pos == '/' &&
          *(token_pos + 1) == '*') {
        token_pos++;
        inBlockComment = true;
      }

      if (has_whitespace() | inLineComment | inBlockComment) {
        token_pos++;
      } else
        break;
    }

    if (*token_pos == ';') {
      token_val[0] = ';';
      token_pos++;
      token_type = DELIMITER;
      return DELIMITER;
    }
    if (*token_pos == '(') {
      token_val[0] = '(';
      token_pos++;
      token_type = DELIMITER;
      return DELIMITER;
    }
    if (*token_pos == ')') {
      token_val[0] = ')';
      token_pos++;
      token_type = DELIMITER;
      return DELIMITER;
    }
    if (*token_pos == ',') {
      token_val[0] = ',';
      token_pos++;
      token_type = DELIMITER;
      return DELIMITER;
    }

    if (*token_pos == '{') {
      token_val[0] = '{';
      token_pos++;
      token_type = BLOCK;
      return BLOCK;
    }
    if (*token_pos == '}') {
      token_val[0] = '}';
      token_pos++;
      token_type = BLOCK;
      return BLOCK;
    }
  }

  // Now check for tokens
  if (is_keyword()) {
    token_type = KEYWORD;
    return KEYWORD;
  } else if (is_builtin_type()) {
    token_type = KEYWORD;
    return BUILTIN_TYPE;
  } else if (is_operator()) {
    token_type = DELIMITER;
    return DELIMITER;
  } else if (is_float()) {
    token_type = FLOAT;
    return FLOAT;
  } else if (is_hex()) {
    token_type = HEX;
    return HEX;
  } else if (is_binary()) {
    token_type = BINARY;
    return BINARY;
  } else if (is_integer()) {
    token_type = INTEGER;
    return INTEGER;
  } else if (is_char()) {
    token_type = CHAR;
    return CHAR;
  } else if (is_string()) {
    token_type = STRING;
    return STRING;
  } else if (is_identifier()) {
    token_type = IDENTIFIER;
    return IDENTIFIER;
  }

  if (*token_pos == '=' && !inString) {
    token_val[0] = '=';
    token_pos++;
    token_type = ASSIGNMENT;
    return ASSIGNMENT;
  }

  token_error = INVALID_SEQUENCE;
  return UNKNOWN;
}

bool is_float(void) {
  bool is_num = false;
  for (int i = 0; i < TKN_MAX_LEN; i++) {
    bool local_is_num = false;

    for (int j = 0; j < strlen(floatsVals); j++)
      if (floatsVals[j] == *token_pos) {
        token_val[i] = *token_pos;
        local_is_num = true;
        is_num = true;
        break;
      }

    if (local_is_num)
      token_pos++;

    if (!local_is_num)
      return is_num;
  }
  return false;
}

bool is_hex(void) {
  if (*token_pos == '0' && *(token_pos + 1) == 'x') {
    // Read until non-binary character + 3 type characters
    token_pos += 2;
    bool is_num = false;
    for (int i = 0; i < TKN_MAX_LEN; i++) {
      bool local_is_num = false;

      for (int j = 0; j < strlen(hexVals); j++)
        if (hexVals[j] == *token_pos) {
          token_val[i] = *token_pos;
          local_is_num = true;
          is_num = true;
          break;
        }
      token_pos++;

      if (!local_is_num)
        return is_num;
    }

    // Token was too long
    token_error = UNKNOWN;
    return true;
  }

  return false;
}

bool is_binary(void) {
  if (*token_pos == '0' && *(token_pos + 1) == 'b') {
    // Read until non-binary character + 3 type characters
    token_pos += 2;
    bool is_num = false;
    for (int i = 0; i < TKN_MAX_LEN; i++) {
      bool local_is_num = false;

      for (int j = 0; j < strlen(binaryVals); j++)
        if (binaryVals[j] == *token_pos) {
          token_val[i] = *token_pos;
          local_is_num = true;
          is_num = true;
          break;
        }
      token_pos++;

      if (!local_is_num)
        return is_num;
    }

    // Token was too long
    token_error = UNKNOWN;
    return true;
  }

  return false;
}

bool is_integer(void) {
  bool is_num = false;
  for (int i = 0; i < TKN_MAX_LEN; i++) {
    bool local_is_num = false;

    for (int j = 0; j < strlen(integersVals); j++)
      if (integersVals[j] == *token_pos) {
        token_val[i] = *token_pos;
        local_is_num = true;
        is_num = true;
        break;
      }

    if (local_is_num)
      token_pos++;

    if (!local_is_num)
      return is_num;
  }
  return false;
}

bool is_char(void) {
  if (*token_pos == '\'') {
    int char_cnt = 1;

    for (int i = 0; i < char_cnt; i++) {
      token_pos++;
      if (*token_pos == 0) {
        token_error = UNEXPECTED_END;
        return true; // Returning true forces this error to be reported instead
                     // of invalid identifier
      }

      if (i == 0)
        char_cnt += (*token_pos == '\\');
      token_val[i] = *token_pos;
    }

    if (*token_pos != '\'') {
      // Check for end of character
      token_error = EXPECTED_TERMINATOR;
      return true;
    }

    token_pos++;
    return true;
  }
  return false;
}

bool is_string(void) {
  if ((!inString && *token_pos == '"') || inString) {
    int tkn_val_cursor = 0;
    if (!inString)
      token_pos++;
    inString = true;
    while (tkn_val_cursor < TKN_MAX_LEN) {
      if (*token_pos == 0) {
        token_error = UNEXPECTED_END; // Source ended without ending the string
        return true;
      }

      if (*token_pos == '"' && !isEscape) { // Check for the end of the
                                            // string
        inString = false;                   // String has ended
        token_pos++;
        break;
      }

      token_val[tkn_val_cursor++] = *token_pos;

      isEscape = false;
      if (*token_pos == '\\' && !isEscape)
        isEscape = true;

      token_pos++;
    }

    return true;
  } else
    return false;
}

bool is_identifier(void) {

  bool isIdent = false;
  for (int i = 0; i < TKN_MAX_LEN; i++) {

    bool local_isIdent = false;
    for (int j = 0; j < strlen(identVals); j++)
      if (*token_pos == identVals[j]) {
        token_val[i] = *token_pos;
        isIdent = true;
        local_isIdent = true;
      }

    if (!local_isIdent)
      return isIdent;
    token_pos++;
  }

  return false;
}

bool is_keyword(void) {
  char **key = keyword_strs;
  int i = 0;

  while (*key != NULL) {
    if (strncmp(*key, token_pos, strlen(*key)) == 0 &&
        check_whitespace(*(token_pos + strlen(*key)))) {
      strcpy(token_val, *key);
      token_detail = i;
      token_pos += strlen(*key);
      return true;
    }

    i++;
    key++;
  }

  return false;
}

bool is_builtin_type(void) {
  char **key = builtin_type_strs;
  int i = 0;

  while (*key != NULL) {
    if (strncmp(*key, token_pos, strlen(*key)) == 0 &&
        check_whitespace(*(token_pos + strlen(*key)))) {
      strcpy(token_val, *key);
      token_detail = i;
      token_pos += strlen(*key);
      return true;
    }
    i++;
    key++;
  }
  return false;
}

bool is_operator(void) {
  char **key = operator_strs;
  int i = 0;

  while (*key != NULL) {
    if (strncmp(*key, token_pos, strlen(*key)) == 0) {
      strcpy(token_val, *key);
      token_detail = i;
      token_pos += strlen(*key);
      return true;
    }
    i++;
    key++;
  }
  return false;
}
