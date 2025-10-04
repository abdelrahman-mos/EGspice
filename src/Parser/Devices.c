#include "../../include/Parser/Devices/device.h"

void vsource_stamp() {
    return;
}

void isource_stamp() {
    return;
}

void resistor_stamp() {
    return;
}

void free_device(Device* device) {
    free(device->device_data);
    free(device);
}