#include "sttp.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("Not enough arguments\n");
    return 1;
  }
  char *root = argv[1];
  sttp(root);
  return 0;
}
