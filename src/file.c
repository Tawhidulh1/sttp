#include <stdio.h>
#include <stdlib.h>
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

char *getFileContents(FILE *fp) {
  if (fseek(fp, 0, SEEK_END) != 0) {
    return NULL;
  }
  long fileSize = ftell(fp);
  char *str = malloc(fileSize + 1);
  if (str == NULL || fileSize < 0) {
    fprintf(stderr, "Error getting fileContents as String");
    free(str);
    return NULL;
  }
  rewind(fp);
  size_t size = fread(str, 1, fileSize, fp);
  if ((long)size < fileSize) {
    fprintf(stderr, "Error getting fileContents as String");
    free(str);
    return NULL;
  }
  str[fileSize] = '\0';
  return str;
}
