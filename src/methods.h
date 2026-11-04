#ifndef METHODS_H
#define METHODS_H

typedef int (*action)(char *response, int repsonseLength, char *method,
                      char *path, char *args);

typedef struct {
  char *method;
  char *path;
  char *args;
  action action;
} route;

int sendBadRequest(int fd, int flags);
int send404(int fd, int flags);
action getAction(char *str);

#endif
