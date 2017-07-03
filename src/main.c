#include "grammar.h"
#include "tokenizer.h"
#include <stdio.h>
#include <string.h>

int main() {

  char *test_str = " if ( int x == 0 )";

  set_tokenizer_data(test_str, strlen(test_str));

  while (true) {
    printf(": %d\r\n", next_token());

    if (get_token_error() == END_FILE)
      break;
  }
}