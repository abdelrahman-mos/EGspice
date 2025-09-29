#ifndef STR_UTILS_H
#define STR_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *my_strdup(const char *s);
char** splittext(const char s[], char split_token[]);
char* _regex_replace_compiled(const regex_t* regex, const char* text, const char* replacement);
char* regex_replace(const char* pattern, const char* text, const char* replacement);
#endif