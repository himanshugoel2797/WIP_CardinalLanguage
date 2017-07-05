#include "grammar.h"
#include "tokenizer.h"
#include <stdio.h>
#include <string.h>

int main() {

  char *test_str = "import System; namespace test { public class test2 { } "
                   "public struct test3 { } }";

  set_tokenizer_data(test_str, strlen(test_str));

  generate_parsetree();
}