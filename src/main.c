#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "methods.h"
#include "sttp.h"

int main(int argc, char *argv[]) {
  int opt;
  char *configPath = NULL;
  char *root = NULL;
  while ((opt = getopt(argc, argv, "c:r:")) != -1) {
    switch (opt) {
    case ('c'):
      configPath = strdup(optarg);
      break;
    case ('r'):
      root = strdup(optarg);
      break;
    }
  }

  if (argc <= 1) {
    printf("Not enough arguments\n");
    return -1;
  }

  if (root == NULL) {
    fprintf(stderr, "invalid root path\n");
    return -1;
  }

  if (configPath == NULL) {
    fprintf(stderr, "invalid config path\n");
    return -1;
  }

  FILE *fp = fopen(configPath, "r");
  if (fp == NULL) {
    fprintf(stderr, "invalid config file\n");
    return -1;
  }

  // TODO: put each route into an array to be passed on to future functions.
  char line[1024];
  route routes[1024];
  int i = 0;
  while ((fgets(line, sizeof line, fp)) != NULL) {
    char method[16], path[2048], action[32], args[2048];
    line[strcspn(line, "\r\n")] = '\0';
    if (sscanf(line, "%15s %2047s %31s %2047s", method, path, action, args) !=
        4) {
      fprintf(stderr, "Error fetching from config file\n");
      return -1;
    }

    route route;
    route.method = strdup(method);
    route.path = strdup(path);
    route.args = strdup(args);
    route.action = getAction(action);

    routes[i] = route;
    i++;
  }

  printf("%s\n", root);
  sttp(root, routes, i);
  return 0;
}
