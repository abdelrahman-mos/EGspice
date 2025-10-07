#include "../../include/Simulator/simulator.h"
#include <time.h>


void run_simulator(char* netlist_path) {
    time_t time_1, time_2;
    time_1 = time(NULL);
    FILE* logfile = fopen("output.log", "w+");
    Netlist* parsed_netlist = parse_netlist(netlist_path, logfile);
    printf("num nodes = %d\nnum vsources = %d\nnum inductors = %d\n", 
        parsed_netlist->num_nodes,
        parsed_netlist->num_vsources,
        parsed_netlist->num_inductors
    );
    run_analyses(parsed_netlist, logfile);
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
    time_2 = time(NULL);
    fprintf(logfile, "time taken %.2lf seconds\n", difftime(time_2, time_1));
    fclose(logfile);
}