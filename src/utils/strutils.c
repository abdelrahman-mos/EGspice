#include "../../include/utils/strutils.h"

char* my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

char** splittext(const char s[], char split_token[]) {
    int num_tokens = 0;
    for (size_t i = 0; i < strlen(s); i++) {
        char tmp = s[i];
        if (tmp == split_token[0]) num_tokens++;
    }
    if (num_tokens == 0) return NULL;

    char* tmp_string = my_strdup(s);
    char** tokens = calloc(num_tokens+2, sizeof(char*));
    char* token = strtok(tmp_string, split_token);
    int iter = 0;

    while (token != NULL)
    {
        tokens[iter++] = my_strdup(token);
        token = strtok(NULL, split_token);
    }

    tokens[iter] = NULL;
    free(tmp_string);
    return tokens;
}