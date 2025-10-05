#include "../../include/Parser/Devices/device.h"
#include <stdlib.h>

void vsource_stamp(Matrix* coeff, Matrix* outputs, Vsource* device) {
    return;
}

void isource_stamp(Matrix* coeff, Matrix* outputs, Isource* device) {
    return;
}

void resistor_stamp(Matrix* coeff, Matrix* outputs, Resistor* device) {
    if (device->node1 != 0) {
        coeff->pValues[device->node1-1][device->node1-1] += 1.0/device->val;
    }

    if (device->node2 != 0) {
        coeff->pValues[device->node2-1][device->node2-1] += 1.0/device->val;
    }

    if ((device->node1 != 0) && (device->node2 != 0)) {
        coeff->pValues[device->node1-1][device->node2-1] -= 1.0/device->val;
        coeff->pValues[device->node2-1][device->node1-1] -= 1.0/device->val;
    }
    return;
}

void stamp_device(Matrix* coeff_matrix, Matrix* outputs_matrix, Device* device) {
    if(device->type == VSOURCE) {
        Vsource* curr_device = (Vsource*) device->device_data;
        return vsource_stamp(coeff_matrix, outputs_matrix, curr_device);
    }
    if (device->type == ISOURCE) {
        Isource* curr_device = (Isource*) device->device_data;
        return isource_stamp(coeff_matrix, outputs_matrix, curr_device);
    }
    if (device->type == RESISTOR) {
        Resistor* curr_device = (Resistor*) device->device_data;
        return resistor_stamp(coeff_matrix, outputs_matrix, curr_device);
    }
}

void free_device(void* device) {
    Device* curr_device = (Device*) device;
    free(curr_device->device_data);
    free(curr_device);
}