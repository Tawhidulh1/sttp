#include <sys/socket.h>

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
