#include "../../include/utils/strutils.h"

char* my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

char** strdup_arr(const char** s) {
    if (s == NULL) return;
    char** copy = NULL;
    int i;
    for (i = 0; s[i] != NULL; i++) {
        copy = realloc(copy, (i+1)*sizeof(char*));
        if (!copy) return NULL;
        copy[i] = my_strdup(s[i]);
    }
    copy = realloc(copy, (i+1)*sizeof(char*));
    if (!copy) return NULL;
    copy[i] = NULL;
    return copy;
}

char** splittext(const char s[], char split_token[]) {
    int num_tokens = 0;
    for (size_t i = 0; i < strlen(s); i++) {
        char tmp = s[i];
        if (tmp == split_token[0]) num_tokens++;
    }
    char** tokens = calloc(num_tokens+2, sizeof(char*));
    if (num_tokens == 0) {
        tokens[0] = s;
        tokens[1] = NULL;
        return tokens;
    }

    char* tmp_string = my_strdup(s);
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

void free_split_text(char** split_text) {
    if (split_text == NULL) return;
    int i;
    for (i = 0; split_text[i] != NULL; i++) {
        free(split_text[i]);
        split_text[i] = NULL;
    }
    free(split_text[i]);
    split_text[i] = NULL;
    free(split_text);
    split_text = NULL;
}

void remove_char_element(char** text_arr, int index) {
    if (!text_arr) return;

    free(text_arr[index]);

    for (int i = index; text_arr[i] != NULL; i++) {
        text_arr[i] = text_arr[i+1];
    }
}

char* strntondup(const char* src, int start, int end) {
    if (end <= start) return NULL;
    char* out = malloc(sizeof(char)*(end-start));
    for (int i = start; i < end; i++) {
        out[i-start] = src[i];
    }
    return out;
}

void lower_str_in_place(char* s) {
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        char tmp = s[i];
        if (isupper(tmp)) {
            s[i] = tolower(tmp);
        }
    }
}

char* _regex_replace_compiled(const regex_t* regex, const char* text, const char* replacement) {
    regmatch_t match;
    size_t repl_len = strlen(replacement);
    size_t out_size = 1; // for '\0'
    char* out = malloc(out_size);
    if (!out) return NULL;
    out[0] = '\0';

    const char* cursor = text;
    // replacement for all matches
    while (regexec(regex, cursor, 1, &match, 0) == 0) {
        size_t before_len = match.rm_so;
        size_t old_len = strlen(out);
        size_t new_size = old_len + before_len + repl_len + 1;

        out = realloc(out, new_size);
        if (!out) return NULL;

        strncat(out, cursor, before_len);
        strcat(out, replacement);
        cursor += match.rm_eo;
    }

    // add the remaining
    size_t old_len = strlen(out);
    size_t tail_len = strlen(cursor);
    out = realloc(out, old_len + tail_len + 1);
    if (!out) return NULL;
    strcat(out, cursor);

    return out;
}

char* regex_replace(const char* pattern, const char* text, const char* replacement, int flags) {
    regex_t regex;
    if (regcomp(&regex, pattern, flags)) {
        fprintf(stderr, "Invalid regex: %s\n", pattern);
        return NULL;
    }
    
    char* result = _regex_replace_compiled(&regex, text, replacement);
    regfree(&regex);
    return result;
}