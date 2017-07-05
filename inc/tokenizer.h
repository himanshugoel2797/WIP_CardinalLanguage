#ifndef CUSTINTER_TOKENIZER_H_
#define CUSTINTER_TOKENIZER_H_

#include <stdbool.h>

int get_token_type(void);

int get_token_error(void);

int get_token_detail(void);

const char *get_token_val(void);

void set_tokenizer_data(const char *data, int len);

bool has_whitespace(void);

int peek_token(void);

int revert_peek_token(void);

int apply_peek_token(void);

int next_token(void);

bool is_float(void);

bool is_hex(void);

bool is_binary(void);

bool is_integer(void);

bool is_char(void);

bool is_string(void);

bool is_identifier(void);

bool is_keyword(void);

bool is_visibility_mod(void);

bool is_mod(void);

bool is_creation(void);

bool is_builtin_type(void);

bool is_operator(void);

#endif
