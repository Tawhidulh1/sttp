#include <asm-generic/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "file.h"

#define PORT "8080"
#define BACKLOG 10
int main() {

  char root[256];
  snprintf(root, sizeof root, "%s/Documents/test", getenv("HOME"));

  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int getAddrInfo = getaddrinfo(NULL, PORT, &hints, &res);
  if (getAddrInfo != 0) {
    fprintf(stderr, "getaddrinfo failed:%s\n", gai_strerror(getAddrInfo));
    return 1;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  int yes = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

  int bindStatus = bind(sockfd, res->ai_addr, res->ai_addrlen);

  if (bindStatus != 0) {
    perror("bind");
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
    }
    printf("Here is the requested data: %s\n", requestData);

    char badRequest[] = "HTTP/1.1 400 Bad Request\r\n"
                        "Content-Length: 11\r\n"
                        "Content-type: text/html\r\n"
                        "\r\n"
                        "Bad Request";

    char method[16], path[2048];
    if (sscanf(requestData, "%15s %2047s", method, path) != 2) {
      send(requestfd, badRequest, strlen(badRequest), 0);
      close(requestfd);
      continue;
    }

    char responseData[4096];
    memset(responseData, 0, sizeof responseData);
    int responseLength = sizeof responseData;

    char pathToIndex[267];
    snprintf(pathToIndex, sizeof pathToIndex, "%s/index.html", root);
    FILE *fp = fopen(pathToIndex, "rb");
    if (fp == NULL) {
      send(requestfd, badRequest, strlen(badRequest), 0);
      close(requestfd);
      continue;
    }
    char *body = getFileContents(fp);
    if (body == NULL) {
      send(requestfd, badRequest, strlen(badRequest), 0);
      close(requestfd);
      continue;
    }
    fclose(fp);
    // TODO: Consider adding Date header
    snprintf(responseData, sizeof responseData,
             "HTTP/1.1 200 OK\r\n"
             "Content-Length: %zu\r\n"
             "Content-type: %s\r\n"
             "\r\n"
             "%s",
             strlen(body), getContentType(pathToIndex), body);
    // strlen(body), "text/html", body);
    int sendCode = send(requestfd, responseData, responseLength, 0);
    if (sendCode == -1) {
      perror("send");
    }
    free(body);
    close(requestfd);
  }
  close(sockfd);
}
