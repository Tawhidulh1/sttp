#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "methods.h"

int redirect(char *response, char *method, char *path, char *args);
int sendBadRequest(int fd, int flags);
int send404(int fd, int flags);

action getAction(char *str) {
  if (strcmp(str, "redirect") == 0) {
    return redirect;
  }
  return redirect;
}

int redirect(char *response, char *method, char *path, char *args) {
  printf("ran redirect\n");
  return 0;
}

int sendBadRequest(int fd, int flags) {
  char badRequestBody[] = "HTTP/1.1 400 Bad Request\r\n"
                          "Content-Length: 11\r\n"
                          "Content-type: text/html\r\n"
                          "\r\n"
                          "Bad Request";
  send(fd, badRequestBody, sizeof badRequestBody - 1, flags);
  return 0;
}

int send404(int fd, int flags) {
  char _404Body[] = "HTTP/1.1 404 Not Found\r\n"
                    "Content-Length: 3\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "404";
  send(fd, _404Body, sizeof _404Body - 1, flags);
  return 0;
}
