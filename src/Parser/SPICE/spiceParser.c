#include "../../../include/Parser/SPICE/spiceParser.h"
#include "../../../include/utils/strutils.h"
#include <stdlib.h>
#include <ctype.h>

spiceParser* parse_netlist(char* netlist_path) {
    spiceParser* parsed_netlist = malloc(sizeof(spiceParser));
    char* netlist_text = read_netlist_file(netlist_path);
    
    char** netlist_text_split = splittext(netlist_text, "\n");
    printf("input netlist:\n%s\n", netlist_text);
    char** netlist_text_split_no_comments = remove_comments(netlist_text_split);
    printf("returned split text\n");
    int i;
    for (i = 0; netlist_text_split[i] != NULL; i++) {
        printf("index %d: %s\n", i, netlist_text_split[i]);
    }
    for (i = 0; netlist_text_split[i] != NULL; i++) {
        free(netlist_text_split[i]);
    }
    free(netlist_text_split[i]);
    free(netlist_text_split);
    // free(netlist_text_split_no_comments);
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
    for (int i = 0; netlist_text_split[i] != NULL; i++) {
        int j = 0;
        char* tmp_text = netlist_text_split[i];
        while (isspace(tmp_text[j])) {
            j++;
        }
        if (tmp_text[j] == '*') {
            printf("This is a comment: %s\n", tmp_text);
        }
    }
    return netlist_text_split;
}

char* parse_options(spiceParser* parser, char* netlist_text) {
    return;
}

char* parse_analyses(spiceParser* parser, char* netlist_text) {
    return;
}

char* parse_devices(spiceParser* parser, char* netlist_text) {
    return;
}

void free_parser(spiceParser* parser) {
    hashmap_destroy(parser->analyses);
    hashmap_destroy(parser->devices);
    hashmap_destroy(parser->options);
    free(parser);
}