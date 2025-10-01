#ifndef DEVICE_H
#define DEVICE_H
#include "Resistor.h"
#include "Vsource.h"
#include "Isource.h"
#include "device_type.h"

#define print_device(device)\
    printf("name: %s, val: %f, node1: %d, node2: %d\n", device->name, device->val, device->node1, device->node2);

typedef struct {
    device_type type;
    void* device_data;
} Device;

#endif