#include "../../include/Simulator/simulator.h"


void run_simulator(char* netlist_path) {
    Netlist* parsed_netlist = parse_netlist(netlist_path);
    printf("num nodes = %d\nnum vsources = %d\n", parsed_netlist->num_nodes, parsed_netlist->num_vsources);
    run_analyses(parsed_netlist);
    // char* analysis = (char*)hashmap_get(parsed_netlist->analyses, "an1");
    // printf("analysis: %s\n", analysis);
    // char** devices_names = hashmap_keys(parsed_netlist->devices);
    // for (int i = 0; devices_names[i] != NULL; i++) {
    //     char* device_name = devices_names[i];
    //     Device* device = hashmap_get(parsed_netlist->devices, device_name);
    //     if (device->type == VSOURCE) {
    //         Vsource* device_data = (Vsource*) device->device_data;
    //         print_device(device_data);
    //     } else if (device->type == ISOURCE)
    //     {
    //         Isource* device_data = (Isource*) device->device_data;
    //         print_device(device_data);
    //     } else if (device->type == RESISTOR)
    //     {
    //         Resistor* device_data = (Resistor*) device->device_data;
    //         print_device(device_data);
    //     } else if (device->type == CAPACITOR)
    //     {
    //         Capacitor* device_data = (Capacitor*) device->device_data;
    //         print_device(device_data);
    //     } else if (device->type == INDUCTOR)
    //     {
    //         Inductor* device_data = (Inductor*) device->device_data;
    //         print_device(device_data);
    //     }      
    // }
    // for (int i = 0; parsed_netlist->nodes[i] != NULL; i++) {
    //     printf("node num %d corresponds to node name %s\n", i, parsed_netlist->nodes[i]);
    // }
    free_parser(parsed_netlist);
}