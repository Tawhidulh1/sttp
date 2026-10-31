#ifndef FILE_H
#define FILE_H

#include <stdio.h>

char *getContentType(char *path);
char *getFileContents(FILE *fp);

#endif
