#include "../../include/Parser/Devices/device.h"
#include <stdlib.h>

void vsource_stamp() {
    return;
}

void isource_stamp() {
    return;
}

void resistor_stamp() {
    return;
}

void free_device(void* device) {
    Device* curr_device = (Device*) device;
    free(curr_device->device_data);
    free(curr_device);
}