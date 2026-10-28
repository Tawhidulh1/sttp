#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = AI_PASSIVE;

  status = getaddrinfo(NULL, "80", &hints, &servinfo);
  int sock = socket(AF_UNSPEC, SOCK_STREAM, 0);

  if (status != 0) {
    exit(1);
  }
}
