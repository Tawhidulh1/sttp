#include "sttp.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc <= 0) {
    printf("Not enough arguments\n");
    sttp(NULL);
  }
  char *root = argv[1];
  sttp(root);
  return 0;
}
