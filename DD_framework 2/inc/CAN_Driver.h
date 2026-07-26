#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>
#include "driver.h"

typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} CAN_Frame;

int CAN_Init(void);
int CAN_Read(void *buffer);
int CAN_Write(void *buffer);
int CAN_Close(void);

extern DriverOps canDriver;

#endif
