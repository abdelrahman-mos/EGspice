#include "../../../include/Parser/SPICE/spiceParser.h"
#include "../../../include/utils/strutils.h"

int cmp_func(const void* a, const void* b) {
    return strcmp((const char *) a, (const char *) b);
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
    char** netlist_text_split_no_options = strdup_arr(netlist_text_split);
    parser->options = hashmap_create(16, hash_string, cmp_func, free, free);
    
    for (int i = 0; netlist_text_split_no_options[i] != NULL; i++) {
        char* curr_line = regex_replace("\\s*=\\s*", netlist_text_split_no_options[i], "=");
        int j = 0;
        while(isspace(curr_line[j])) j++;
        if (curr_line[j] != '.') continue;
        char** curr_line_splitted = splittext(curr_line, " ");

        char* option = curr_line_splitted[0];
        if (!option) return NULL;
        lower_str_in_place(option);


        if (strcmp(option, ".option") != 0) {
            continue;
        } 

        for (int i = 1; curr_line_splitted[i] != NULL; i++) {
            option = curr_line_splitted[i];
            char flag = 0;
            for (int j = 0; j < strlen(option); j++) {
                if (option[j] == '=') flag = 1;
            }
            if (flag) {
                char** option_split = splittext(option, "=");
                hashmap_insert(parser->options, option_split[0], option_split[1]);
            } else {
                hashmap_insert(parser->options, option, my_strdup(""));  
            }     
        }
        remove_char_element(netlist_text_split_no_options, i);
        
    }
    return netlist_text_split_no_options;
}

char** parse_analyses(spiceParser* parser, char** netlist_text_split) {
    char** netlist_text_split_no_analyses = strdup_arr(netlist_text_split);
    parser->analyses = hashmap_create(16, hash_string, cmp_func, free, free);
    
    for (int i = 0; netlist_text_split_no_analyses[i] != NULL; i++) {
        char* curr_line = netlist_text_split_no_analyses[i];
        int j = 0;
        int num_analysis = 0;
        char analysis_name[5];
        while(isspace(curr_line[j])) j++;
        if (curr_line[j] != '.') continue;
        char** curr_line_splitted = splittext(curr_line, " ");
        char* analysis = my_strdup(curr_line_splitted[0]);
        if (!analysis) return NULL;
        lower_str_in_place(analysis);


        if (strcmp(analysis, ".op") != 0) {
            fprintf(stderr, "unsupported analysis %s\n", analysis);
            return NULL;
        } 
        sprintf(analysis_name, "an%d", ++num_analysis);
        hashmap_insert(parser->analyses, my_strdup(analysis_name), my_strdup(curr_line));
        remove_char_element(netlist_text_split_no_analyses, i);
        
    }
    return netlist_text_split_no_analyses;
}

void _add_node(spiceParser* parser, char* node_name, int* node_to_add, int* curr_node) {
    int i;
    for (i = 1; parser->nodes[i] != NULL; i++) {
        if (strcmp(node_name, parser->nodes[i]) == 0) {
            *node_to_add = i;
            return;
        }
    }
    // here, i will map to the size of nodes
    parser->nodes = (char**)realloc(parser->nodes, (i+2)*sizeof(char*));
    parser->nodes[i] = node_name;
    parser->nodes[i+1] = NULL; // ????
    *node_to_add = *curr_node;
    *curr_node = *curr_node + 1;
}

void parse_two_terminal_device(spiceParser* parser, char* line, device_type type) {
    char** line_split = splittext(line, " ");
    static int node = 1;
    printf("node num: %d\n", node);
    static char gnd_found = 0;
    int node_1_to_add = -1;
    int node_2_to_add = -1;
    int line_len = 0;
    for (line_len = 0; line_split[line_len] != NULL; line_len++);
    if (line_len != 4) {
        fprintf(stderr, "Incorrect device definition %s", line);
        return;
    }
    
    char* device_name = line_split[0];
    // need to support non-number node names
    char* node_1 = line_split[1];
    char* node_2 = line_split[2];
    if (strcmp(node_1, node_2) == 0) return; // shorted device lol
    if ((strcmp(node_1, "0") == 0) || (strcmp(node_1, "gnd") == 0)) {
        node_1_to_add = 0;
        gnd_found = 1;
    } 
    
    if ((strcmp(node_2, "0") == 0) || (strcmp(node_2, "gnd") == 0)) {
        node_2_to_add = 0;
        gnd_found = 1;
    }

    if (node_1_to_add == -1) {
        _add_node(parser, node_1, &node_1_to_add, &node);
    }

    if (node_2_to_add == -1) {
        _add_node(parser, node_2, &node_2_to_add, &node);
    }
    
    // need to support postfix removal and replacing
    // 1v -> 1
    // 1a -> 1e-18
    // 1k -> 1000
    // 1m -> 0.001
    // 1meg -> 1000000
    double val = atof(line_split[3]);
    if (type == VSOURCE) {
        Device* device = malloc(sizeof(Device));
        device->type = VSOURCE;
        Vsource* device_data = (Vsource*) malloc(sizeof(Vsource));
        device_data->name = device_name;
        device_data->node1 = node_1_to_add;
        device_data->node2 = node_2_to_add;
        device_data->val = val;
        device->device_data = device_data;
        hashmap_insert(parser->devices, device_name, device);
    } else if (type == ISOURCE)
    {
        Device* device = malloc(sizeof(Device));
        device->type = ISOURCE;
        Isource* device_data = malloc(sizeof(Isource));
        device_data->name = device_name;
        device_data->node1 = node_1_to_add;
        device_data->node2 = node_2_to_add;
        device_data->val = val;
        device->device_data = device_data;
        hashmap_insert(parser->devices, device_name, device);
    } else if (type == RESISTOR) {
        Device* device = malloc(sizeof(Device));
        device->type = RESISTOR;
        Resistor* device_data = malloc(sizeof(Resistor));
        device_data->name = device_name;
        device_data->node1 = node_1_to_add;
        device_data->node2 = node_2_to_add;
        device_data->val = val;
        device->device_data = device_data;
        hashmap_insert(parser->devices, device_name, device);
    }
}

void parse_devices(spiceParser* parser, char** netlist_text_split) {
    char** netlist_text_split_no_devices = strdup_arr(netlist_text_split);
    parser->devices = hashmap_create(16, hash_string, cmp_func, free, free_device);
    parser->nodes = (char**)calloc(2, sizeof(char*));
    parser->nodes[0] = my_strdup("0");
    parser->nodes[1] = NULL;
    for (int i = 0; netlist_text_split_no_devices[i] != NULL; i++) {
        char* curr_line = netlist_text_split_no_devices[i];
        lower_str_in_place(curr_line);
        int j = 0;
        while(isspace(curr_line[j])) j++;
        if ((curr_line[j] != 'v') && (curr_line[j] != 'r') && (curr_line[j] != 'i')) {
            fprintf(stderr, "unsupported device %s\n", curr_line);
            return;
        }
        device_type type;
        switch (curr_line[j])
        {
        case 'v':
            type = VSOURCE;
            break;
        case 'i':
            type = ISOURCE;
            break;
        case 'r':
            type = RESISTOR;
            break;
        }
        parse_two_terminal_device(parser, curr_line, type);
    }
}

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
    // printf("text with no comments nor analyses\n");
    // for (int i = 0; netlist_text_split_no_analyses[i] != NULL; i++) {
    //     printf("index %d: %s\n", i, netlist_text_split_no_analyses[i]);
    // }
    // printf("text with no comments nor analyses nor options\n");
    // for (int i = 0; netlist_text_split_no_options[i] != NULL; i++) {
    //     printf("index %d: %s\n", i, netlist_text_split_no_options[i]);
    // }
    free_split_text(netlist_text_split);
    free_split_text(netlist_text_split_no_comments);
    free_split_text(netlist_text_split_no_analyses);
    free_split_text(netlist_text_split_no_options);
    return parsed_netlist;
}

void free_parser(spiceParser* parser) {
    hashmap_destroy(parser->analyses);
    hashmap_destroy(parser->devices);
    hashmap_destroy(parser->options);
    free_split_text(parser->nodes);
    free(parser);
}