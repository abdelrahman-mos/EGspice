#ifndef SPICE_PARSER
#define SPICE_PARSER
#include "../../utils/map.h"
#include "../Devices/device.h"

typedef struct spiceParser
{
    HashMap* devices;
    HashMap* options;
    HashMap* analyses;
} spiceParser;

spiceParser* parse_netlist(char* netlist_path);
char* read_netlist_file(char* netlist_path);
char* remove_comments(char* netlist_text);
char* parse_options(spiceParser* parser, char* netlist_text);
char* parse_analyses(spiceParser* parser, char* netlist_text);
char* parse_devices(spiceParser* parser, char* netlist_text);
void free_parser(spiceParser* parser);


#endif