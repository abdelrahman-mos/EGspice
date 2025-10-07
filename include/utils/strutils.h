#ifndef STR_UTILS_H
#define STR_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define REPLACE_INPLACE(var, regex, repl, flags) \
    do { \
        char* __tmp = regex_replace((regex), (var), (repl), (flags)); \
        free(var); \
        var = __tmp; \
    } while(0)

char* my_strdup(const char* s);
char** strdup_arr(const char** s);
char** splittext(const char s[], char split_token[]);
char* _regex_replace_compiled(const regex_t* regex, const char* text, const char* replacement);
char* regex_replace(const char* pattern, const char* text, const char* replacement, int flags);
char* strntondup(const char* src, int start, int end);
void lower_str_in_place(char* s);
void free_split_text(char** split_text);
void remove_char_element(char** text_arr, int index);
#endif