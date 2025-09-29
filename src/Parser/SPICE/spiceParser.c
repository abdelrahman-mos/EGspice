#include "../../../include/Parser/SPICE/spiceParser.h"
#include "../../../include/utils/strutils.h"

spiceParser* parse_netlist(char* netlist_path) {
    spiceParser* parsed_netlist = malloc(sizeof(spiceParser));
    char* netlist_text = read_netlist_file(netlist_path);
    char** netlist_text_split = splittext(netlist_text, "\n");
    if (!netlist_text_split) return NULL;

    remove_char_element(netlist_text_split, 0);
    char** netlist_text_split_no_comments = remove_comments(netlist_text_split);
    char** netlist_text_split_no_analyses = parse_analyses(parsed_netlist, netlist_text_split_no_comments);
    char** netlist_text_split_no_options = parse_options(parsed_netlist, netlist_text_split_no_analyses);
    parse_devices(parsed_netlist, netlist_text_split_no_options);

    // printf("returned split text\n");
    // for (int i = 0; netlist_text_split[i] != NULL; i++) {
    //     printf("index %d: %s\n", i, netlist_text_split[i]);
    // }
    // printf("text with no comments\n");
    // for (int i = 0; netlist_text_split_no_comments[i] != NULL; i++) {
    //     printf("index %d: %s\n", i, netlist_text_split_no_comments[i]);
    // }

    free_split_text(netlist_text_split);
    free_split_text(netlist_text_split_no_comments);
    free_split_text(netlist_text_split_no_analyses);
    free_split_text(netlist_text_split_no_options);
    return parsed_netlist;
}

char* read_netlist_file(char* netlist_path) {
    FILE* input_netlist_file = fopen(netlist_path, "r");
    if (!input_netlist_file) {
        perror("Failed to open netlist");
        return NULL;
    }

    fseek(input_netlist_file, 0, SEEK_END);
    long size = ftell(input_netlist_file);
    rewind(input_netlist_file);

    char* netlist_text = malloc(size + 1);
    if (!netlist_text) {
        fclose(input_netlist_file);
        return NULL;
    }

    fread(netlist_text, 1, size, input_netlist_file);
    netlist_text[size] = 0;

    fclose(input_netlist_file);
    return netlist_text;
}

char** remove_comments(char** netlist_text_split) {
    char** netlist_text_split_no_comments = strdup_arr(netlist_text_split);
    for (int i = 0; netlist_text_split_no_comments[i] != NULL; i++) {
        int j = 0;
        char* tmp_text = netlist_text_split_no_comments[i];
        // skip blank
        while (isblank(tmp_text[j])) j++;
        if (tmp_text[j] == '*') {
            remove_char_element(netlist_text_split_no_comments, i--);
        }
    }
    return netlist_text_split_no_comments;
}

char** parse_options(spiceParser* parser, char** netlist_text_split) {
    return strdup_arr(netlist_text_split);
}

int cmp_func(const void* a, const void* b) {
    return strcmp((const char *) a, (const char *) b);
}

char** parse_analyses(spiceParser* parser, char** netlist_text_split) {
    char** netlist_text_split_no_analyses = strdup_arr(netlist_text_split);
    parser->analyses = hashmap_create(16, hash_string, cmp_func, free, free);
    
    for (int i = 0; netlist_text_split_no_analyses[i] != NULL; i++) {
        char* curr_line = netlist_text_split_no_analyses[i];
        printf("current line is %s\n", curr_line);
        int j = 0;
        while(isspace(curr_line[j])) j++;
        if (curr_line[j] != '.') continue;
        char** curr_line_splitted = splittext(curr_line, " ");
        for (int i = 0; curr_line_splitted[i] != NULL; i++) {
        printf("index %d: %s\n", i, curr_line_splitted[i]);
    }
        char* analysis = curr_line_splitted[0];
        if (!analysis) return NULL;
        lower_str_in_place(analysis);


        if (strcmp(analysis, ".op") != 0) {
            fprintf(stderr, "unsupported analysis %s\n", analysis);
            return NULL;
        } 

        printf("current line is a supported analysis %s\n", analysis);
        hashmap_insert(parser->analyses, "an1", analysis);
        free(analysis);
    }
    return netlist_text_split_no_analyses;
}

void parse_devices(spiceParser* parser, char** netlist_text_split) {
    return strdup_arr(netlist_text_split);
}

void free_parser(spiceParser* parser) {
    hashmap_destroy(parser->analyses);
    hashmap_destroy(parser->devices);
    hashmap_destroy(parser->options);
    free(parser);
}