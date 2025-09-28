#include "../../include/utils/strutils.h"

char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

char** splittext(const char* s, const char split_token) {
    printf("inside splittext\n");
    int num_tokens = 0;
    for (int i = 0; i < strlen(s); i++) {
        char tmp = s[i];
        printf("tmp char = %c\n", tmp);
        if (tmp == split_token) num_tokens++;
    }
    if (num_tokens == 0) return NULL;
    printf("got number of tokens = %d\n", num_tokens);
    char** tokens = calloc(num_tokens+2, sizeof(char*));
    printf("allocated memory for tokens\n");
    char* token = strtok(s, split_token);
    printf("split the first token\n");
    int iter = 0;
    while (token != NULL)
    {
        printf("token: %s\n", token);
        size_t len = strlen(token) + 1;
        memcpy(tokens[iter++], token, len);
        token = strtok(NULL, split_token);
    }
    printf("iter: %d\n", iter);
    tokens[iter] = "\0";
    return tokens;
}