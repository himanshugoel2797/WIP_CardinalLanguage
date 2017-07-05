#include "grammar.h"
#include "tokenizer.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

  FILE *fd = fopen(argv[1], "r");
  fseek(fd, 0, SEEK_END);
  size_t len = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  char *test_str = malloc(len);
  fread(test_str, 1, len, fd);
  fclose(fd);

  set_tokenizer_data(test_str, strlen(test_str));

  generate_parsetree();
}