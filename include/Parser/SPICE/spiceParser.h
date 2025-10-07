#ifndef SPICE_PARSER
#define SPICE_PARSER
#include "../netlist.h"
#include "../Devices/device.h"

Netlist* parse_netlist(char* netlist_path, FILE* logfile);
// char* read_netlist_file(char* netlist_path);
// char** parse_options(Netlist* parser, char** netlist_text_split);
// char** parse_analyses(Netlist* parser, char** netlist_text_split);
// void parse_devices(Netlist* parser, char** netlist_text_split);
void free_parser(Netlist* parser);


#endif