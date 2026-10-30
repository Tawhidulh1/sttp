#include <string.h>

char *getContentType(char *path) {
  char *extension = strrchr(path, '.');
  if (extension == NULL) {
    return "application/octet-stream";
  }
  if (strcmp(extension, ".html") == 0 || strcmp(extension, ".HTML") == 0) {
    return "text/html";
  }
  return "application/octet-stream";
}
