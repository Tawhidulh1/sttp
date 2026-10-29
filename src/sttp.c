#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "8080"
#define BACKLOG 10

int main() {
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int sockinfo = getaddrinfo(NULL, PORT, &hints, &res);

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  int bindStatus = bind(sockfd, res->ai_addr, res->ai_addrlen);

  if (bindStatus != 0) {
    perror("error with bind");
    exit(1);
  }

  struct sockaddr_in requestAddr;
  socklen_t requestSize;
  while (1) {
    int listenCode = listen(sockfd, BACKLOG);
    if (listenCode != 0) {
      exit(-1);
    }
    printf("Waiting for request\n");
    requestSize = sizeof requestAddr;
    int requestfd =
        accept(sockfd, (struct sockaddr *)&requestAddr, &requestSize);
    printf("(%d)Request accepted\n", sockfd);
    char requestData[4096];
    int bytesRecieved = recv(requestfd, requestData, sizeof requestData - 1, 0);
    if (bytesRecieved > 0) {
      requestData[bytesRecieved] = '\0';
      printf("Recieved: %d bytes\n", bytesRecieved);
      printf("Here is your data sir: \n%s\n", requestData);
    }
  }
}
