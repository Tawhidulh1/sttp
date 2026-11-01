#include <asm-generic/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "file.h"
#include "methods.h"

#define PORT "8080"
#define BACKLOG 10

int sttp(char *root) {
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
  int listenCode = listen(sockfd, BACKLOG);
  if (listenCode != 0) {
    exit(-1);
  }
  printf("Waiting for request\n");
  while (1) {
    requestSize = sizeof requestAddr;
    int requestfd =
        accept(sockfd, (struct sockaddr *)&requestAddr, &requestSize);

    if (requestfd == -1) {
      perror("accept");
      continue;
    }

    printf("(%d)Request accepted\n", sockfd);
    char requestData[4096];
    int bytesRecieved = recv(requestfd, requestData, sizeof requestData - 1, 0);
    if (bytesRecieved > 0) {
      requestData[bytesRecieved] = '\0';
    } else {
      close(requestfd);
    }
    printf("Here is the requested data: %s\n", requestData);

    char method[16], path[2048];
    if (sscanf(requestData, "%15s %2047s", method, path) != 2 || root == NULL) {
      sendBadRequest(requestfd, 0);
      close(requestfd);
      continue;
    }

    // if ".." is found, VERY BAD
    if (strstr(path, "..") != NULL) {
      sendBadRequest(requestfd, 0);
      close(requestfd);
      printf("DANGER DANGER\n");
      continue;
    }

    // default / to /index.html
    if (strcmp(path, "/") == 0) {
      snprintf(path, sizeof path, "/index.html");
    }

    char responseData[4096];
    memset(responseData, 0, sizeof responseData);
    int responseLength = sizeof responseData;

    char pathToFile[strlen(root) + strlen(path) + 2];
    snprintf(pathToFile, sizeof pathToFile, "%s/%s", root, path);
    FILE *fp = fopen(pathToFile, "rb");

    if (fp == NULL) {
      send404(requestfd, 0);
      close(requestfd);
      continue;
    }
    char *body = getFileContents(fp);
    if (body == NULL) {
      send404(requestfd, 0);
      fclose(fp);
      close(requestfd);
      continue;
    }
    fclose(fp);
    // TODO: Consider adding Date header
    if (strcmp(method, "HEAD") == 0) {
      snprintf(responseData, sizeof responseData,
               "HTTP/1.1 200 OK\r\n"
               "Content-Length: %zu\r\n"
               "Content-type: %s\r\n"
               "\r\n",
               strlen(body), getContentType(pathToFile));
    } else if (strcmp(method, "GET") == 0) {
      snprintf(responseData, sizeof responseData,
               "HTTP/1.1 200 OK\r\n"
               "Content-Length: %zu\r\n"
               "Content-type: %s\r\n"
               "\r\n"
               "%s",
               strlen(body), getContentType(pathToFile), body);
    } else {
      snprintf(responseData, sizeof responseData,
               "HTTP/1.1 405 Method Not Allowed\r\n"
               "Allow: GET, HEAD\r\n"
               "\r\n");
    }
    int sendCode = send(requestfd, responseData, responseLength, 0);
    if (sendCode == -1) {
      perror("send");
    }
    free(body);
    close(requestfd);
  }
  close(sockfd);
}
