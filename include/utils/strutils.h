#ifndef STR_UTILS_H
#define STR_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *my_strdup(const char *s);
char** splittext(const char s[], char split_token[]);
#endif