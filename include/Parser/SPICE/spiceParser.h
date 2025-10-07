#ifndef SPICE_PARSER
#define SPICE_PARSER
#include "../netlist.h"
#include "../Devices/device.h"

Netlist* parse_netlist(char* netlist_path, FILE* logfile);
void free_parser(Netlist* parser);


#endif