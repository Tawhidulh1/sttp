#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "methods.h"

int redirect(char *response, int responseLength, char *method, char *path,
             char *args);
int sendBadRequest(int fd, int flags);
int send404(int fd, int flags);

action getAction(char *str) {
  if (strcmp(str, "redirect") == 0) {
    return redirect;
  }
  return redirect;
}

int redirect(char *response, int responseLength, char *method, char *path,
             char *args) {

  snprintf(response, responseLength,
           "HTTP/1.1 302 Found\r\n"
           "Location: %s\r\n"
           "Content-Length: 0\r\n"
           "\r\n",
           args);

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

int sendWithCode(int fd, int errorCode, int flags) {
  char _responseBody[4096];
  char error[128] = {0};
  switch (errorCode) {
  case (404):
    strcpy(error, "Not Found");
    break;
  case (400):
    strcpy(error, "Bad Request");
    break;
  case (501):
    strcpy(error, "Not Implemented");
    break;
  case (505):
    strcpy(error, "HTTP Version Not Supported");
    break;
  default:
    return -1;
  }
  snprintf(_responseBody, sizeof _responseBody,
           "HTTP/1.1 %d %s\r\n"
           "Content-Length: %zu\r\n"
           "Content-type: text/html\r\n"
           "\r\n"
           "%d %s",
           errorCode, error, strlen(error) + 4, errorCode, error);
  send(fd, _responseBody, sizeof _responseBody - 1, flags);
  return 0;
}
