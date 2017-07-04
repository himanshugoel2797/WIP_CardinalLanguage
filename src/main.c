#include "grammar.h"
#include "tokenizer.h"
#include <stdio.h>
#include <string.h>

int main() {

  char *test_str = "set_tokenizer_data(test_str, strlen(test_str)); while (true) { int type = next_token(); printf(\"%s : %d\r\n\", get_token_val(), type); if (get_token_error() == END_FILE) break; }";

      set_tokenizer_data(test_str, strlen(test_str));

  while (true) {

    int type = next_token();

    printf("%s : %d\r\n", get_token_val(), type);

    if (get_token_error() == END_FILE)
      break;
  }
}