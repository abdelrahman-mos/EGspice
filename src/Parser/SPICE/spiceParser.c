#include "../../../include/Parser/SPICE/spiceParser.h"
#include <stdlib.h>

spiceParser* parse_netlist(char* netlist_path) {
    spiceParser* parsed_netlist = malloc(sizeof(spiceParser));
    char* netlist_text = read_netlist_file(netlist_path);
    printf("input netlist:\n%s\n", netlist_text);
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

char* remove_comments(char* netlist_text) {
    return;
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
