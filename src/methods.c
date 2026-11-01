#include <sys/socket.h>

int sendBadRequest(int fd, int flags) {
  char badRequestBody[] = "HTTP/1.1 400 Bad Request\r\n"
                          "Content-Length: 11\r\n"
                          "Content-type: text/html\r\n"
                          "\r\n"
                          "Bad Request";
  send(fd, badRequestBody, sizeof badRequestBody, flags);
  return 0;
}
