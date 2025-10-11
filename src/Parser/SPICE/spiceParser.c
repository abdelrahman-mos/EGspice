#include "../../../include/Parser/SPICE/spiceParser.h"
#include "../../../include/utils/strutils.h"
#include "../../../include/Simulator/Analysis/analysis.h"

const char* supported_options[] = {"gmin", "aex", NULL};

char is_supported_option(char* option) {
    for (int i = 0; supported_options[i] != NULL; i++) {
        if (strcmp(option, supported_options[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int cmp_func(const void* a, const void* b) {
    return strcmp((const char *) a, (const char *) b);
}

Netlist* initialize_netlist() {
    Netlist* output = malloc(sizeof(Netlist));
    output->analyses = NULL;
    output->devices = NULL;
    output->options = NULL;
    output->vsources = NULL;
    output->nodes = NULL;
    output->vsources = NULL;
    output->inductors = NULL;
    output->num_inductors = 0;
    output->num_nodes = 0;
    output->num_vsources = 0;
    return output;
}

char* read_netlist_file(char* netlist_path) {
    FILE* input_netlist_file = fopen(netlist_path, "r");
    if (!input_netlist_file) {
        perror("Failed to open netlist");
        exit(-1);
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

void _add_node(Netlist* parser, char* node_name, int* node_to_add, int* curr_node) {
    int i;
    for (i = 1; parser->nodes[i] != NULL; i++) {
        if (strcmp(node_name, parser->nodes[i]) == 0) {
            *node_to_add = i;
            return;
        }
    }
    // here, i will map to the size of nodes
    parser->nodes = (char**)realloc(parser->nodes, (i+2)*sizeof(char*));
    parser->nodes[i] = my_strdup(node_name);
    parser->nodes[i+1] = NULL; // ????
    *node_to_add = *curr_node;
    *curr_node = *curr_node + 1;
    parser->num_nodes++;
}

double device_val_to_double(char* text) {
    if (!text) return 0.0;
    // is this the best method ? is there a different method ? can I use regex in a different way ?
    char* out = regex_replace("[Aa]+", text, "a", REG_EXTENDED);
    REPLACE_INPLACE(out, "[Ff]+", "f", REG_EXTENDED);
    REPLACE_INPLACE(out, "(Amp|H|V|OHM)$", "", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(MEG)", "e6", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(T)", "e12", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(G)", "e9", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(K)", "e3", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(m)", "e-3", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(u)", "e-6", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(n)", "e-9", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(p)", "e-12", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(f)", "e-15", REG_EXTENDED | REG_ICASE);
    REPLACE_INPLACE(out, "(a)", "e-18", REG_EXTENDED | REG_ICASE);
    double output = atof(out);
    free(out);
    return output;
}

void parse_two_terminal_device(Netlist* parser, char* line, device_type type, FILE* logfile) {
    char** line_split = splittext(line, " ");
    static int node = 1;
    static char gnd_found = 0;
    int node_1_to_add = -1;
    int node_2_to_add = -1;
    int line_len = 0;
    for (line_len = 0; line_split[line_len] != NULL; line_len++);
    if (line_len != 4) {
        fprintf(logfile, "Incorrect device definition %s", line);
        exit(-1);
    }
    
    char* device_name = line_split[0];
    char* device_exists = hashmap_get(parser->devices, device_name);
    if (device_exists != NULL) {
        fprintf(logfile, "Device %s is defined more than once\n", device_name);
        exit(-1);
    }
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
    double val = device_val_to_double(line_split[3]);
    Device* device = malloc(sizeof(Device));
    // is doing this actually a good thing ?
    commonData* data = malloc(sizeof(commonData));
    data->name = device_name;
    data->node1 = node_1_to_add;
    data->node2 = node_2_to_add;
    data->val = val;
    device->type = type;
    if (type == VSOURCE) {
        Vsource* device_data = (Vsource*) data;
        device->device_data = device_data;
        parser->num_vsources++;
    } else if (type == ISOURCE)
    {
        Isource* device_data = (Isource*) data;
        device->device_data = device_data;
    } else if (type == RESISTOR) {
        Resistor* device_data = (Resistor*) data;
        device->device_data = device_data;
    } else if (type == CAPACITOR) {
        Capacitor* device_data = (Capacitor*) data;
        device->device_data = device_data;
    } else if (type == INDUCTOR) {
        Inductor* device_data = (Inductor*) data;
        device->device_data = device_data;
        parser->num_inductors++;
    }
    hashmap_insert(parser->devices, my_strdup(device_name), device);
    free_split_text(line_split);
}

void parse_option(Netlist* parsed_netlist, char** curr_line_splitted, FILE* logfile) {
    if (parsed_netlist->options == NULL) {
        parsed_netlist->options = hashmap_create(16, hash_string, cmp_func, free, free);
    }
    for (int i = 1; curr_line_splitted[i] != NULL; i++) {
        char* option = curr_line_splitted[i];
        char flag = 0;
        for (size_t j = 0; j < strlen(option); j++) {
            if (option[j] == '=') flag = 1;
        }
        lower_str_in_place(option);
        if (flag) {
            char** option_split = splittext(option, "=");
            char* option_name = option_split[0];
            if (!is_supported_option(option_name)) {
                fprintf(logfile, "unsupported option %s\n", option_name);
                free_split_text(option_split);
                continue; // since one option line can contain multiple options
            }
            hashmap_insert(parsed_netlist->options, my_strdup(option_split[0]), my_strdup(option_split[1]));
            free_split_text(option_split);
        } else {
            if (!is_supported_option(option)) {
                fprintf(logfile, "unsupported option %s\n", option);
                continue; // since one option line can contain multiple options
            }
            hashmap_insert(parsed_netlist->options, my_strdup(option), my_strdup(""));  
        }     
    }
}

AC_Analysis* parse_ac_analysis(char** curr_line_splitted, FILE* logfile) {
    AC_Analysis* ac_analysis = (AC_Analysis*)malloc(sizeof(AC_Analysis));
    AC_TYPE ac_analysis_type;
    char* tmp = curr_line_splitted[1];
    if (strcmp(tmp, "dec") == 0) {
        ac_analysis_type = DEC;
    } else if (strcmp(tmp, "oct") == 0) {
        ac_analysis_type = OCT;
    } else if (strcmp(tmp, "lin") == 0) {
        ac_analysis_type = LIN;
    } else {
        fprintf(logfile, "Incorrect AC analysis points type %s\n", tmp);
        free(ac_analysis);
        return NULL;
    }
    int numpoints = atoi(curr_line_splitted[2]); // this must be an integer lol
    double start = device_val_to_double(curr_line_splitted[3]);
    double end = device_val_to_double(curr_line_splitted[4]);
    ac_analysis->type = ac_analysis_type;
    ac_analysis->num_points = numpoints;
    ac_analysis->start = start;
    ac_analysis->end = end;
    return ac_analysis;
}

void parse_analysis(Netlist* parsed_netlist, char** curr_line_splitted, FILE* logfile) {
    if (parsed_netlist->analyses == NULL) {
        parsed_netlist->analyses = hashmap_create(16, hash_string, cmp_func, free, free_analysis);
    }

    static int num_analysis = 0;
    char analysis_name[5];
    Analysis* analysis = malloc(sizeof(Analysis));
    analysis->analysis_data = NULL;
    sprintf(analysis_name, "an%d", ++num_analysis);
    analysis->analysis_name = analysis_name;
    char* analysis_command = curr_line_splitted[0];
    if (strcmp(analysis_command, ".op") == 0) {
        analysis->type = OP;
    } else if (strcmp(analysis_command, ".ac") == 0) {
        // ac type numpoints start end
        int line_len = 0;
        for(; curr_line_splitted[line_len] != NULL; line_len++) {
            lower_str_in_place(curr_line_splitted[line_len]);
        }
        if (line_len != 5) {
            fprintf(logfile, "Incorrect AC analysis definition\n");
            free_analysis(analysis);
            return;
        }
        analysis->type = AC;
        AC_Analysis* ac_analysis = parse_ac_analysis(curr_line_splitted, logfile);
        if (ac_analysis == NULL) {
            free_analysis(analysis);
            return;
        }
        analysis->analysis_data = ac_analysis;
    }
    
    hashmap_insert(parsed_netlist->analyses, my_strdup(analysis_name), analysis);
}

void parse_dot_command(Netlist* parsed_netlist, char* curr_line, FILE* logfile) {
    char* tmp_line = regex_replace("\\s*=\\s*", curr_line, "=", REG_EXTENDED);
    char** curr_line_splitted = splittext(tmp_line, " ");
    char* dot_command = curr_line_splitted[0];
    lower_str_in_place(dot_command);
    if (strcmp(dot_command, ".option") == 0) {
        parse_option(parsed_netlist, curr_line_splitted, logfile);
    } else if (is_supported_analysis(dot_command)) {
        parse_analysis(parsed_netlist, curr_line_splitted, logfile);
    } else {
        fprintf(logfile, "unsupported dot command %s\n", tmp_line);
    }
    free(tmp_line);
    free_split_text(curr_line_splitted);
}

void parse_device(Netlist* parsed_netlist, char* curr_line, FILE* logfile) {
    if (parsed_netlist->devices == NULL) {
        parsed_netlist->devices = hashmap_create(16, hash_string, cmp_func, free, free_device);
        parsed_netlist->nodes = (char**)calloc(2, sizeof(char*));
        parsed_netlist->nodes[0] = my_strdup("0");
        parsed_netlist->nodes[1] = NULL;
    }
    lower_str_in_place(curr_line);
    int j = 0;
    while(isspace(curr_line[j])) j++; // skip spaces
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
    case 'c':
        type = CAPACITOR;
        break;
    case 'l':
        type = INDUCTOR;
        break;
    default:
        fprintf(logfile, "unsupported device %s\n", curr_line);
        return;
    }
    parse_two_terminal_device(parsed_netlist, curr_line, type, logfile);
}

// we currently don't support multiline commands such as using \ or +
Netlist* parse_netlist(char* netlist_path, FILE* logfile) {
    Netlist* parsed_netlist = initialize_netlist();
    char* netlist_text = read_netlist_file(netlist_path);
    char** netlist_text_split = splittext(netlist_text, "\n");
    if (!netlist_text_split) return NULL;
    fprintf(logfile, "Netlist:\n");
    fprintf(logfile, "%s\n\n", netlist_text);
    remove_char_element(netlist_text_split, 0);
    // making parsing sequential instead of repeating operations on lines,
    // this might be a little bit troubling when needing to parse subckts before the netlist itself
    for (int i = 0; netlist_text_split[i] != NULL; i++) {
        char* curr_line = netlist_text_split[i];
        int j;
        for (j = 0; isspace(curr_line[j]); j++); // skip spaces
        if (curr_line[j] == '*') {
            // check first if it's a comment and remove it
            remove_char_element(netlist_text_split, i--);
            continue;
        } else if (curr_line[j] == '.') {
            // check if it's a dot command (option, analysis)
            parse_dot_command(parsed_netlist, curr_line, logfile);
        } else {
            // else, we'll parse it as a device
            parse_device(parsed_netlist, curr_line, logfile);
        }
    }
    free(netlist_text);
    free_split_text(netlist_text_split);
    return parsed_netlist;
}

void free_parser(Netlist* parser) {
    hashmap_destroy(parser->analyses);
    hashmap_destroy(parser->devices);
    hashmap_destroy(parser->options);
    free_split_text(parser->nodes);
    free_split_text(parser->vsources);
    free_split_text(parser->inductors);
    free(parser);
}